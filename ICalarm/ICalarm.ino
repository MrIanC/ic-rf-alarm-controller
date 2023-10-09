/*
  Written By: Ian Cumming
  Purpose:
  Hardware:
    Wemos R1 D2
    Relay Module
    433Mhz Reciever
    Siren Module
    Components
      2 x 270 Ohm
      1 x LED
      Buzzer
  Build:
    RF_PIN1   -  RF Reciever Module to 5v and Ground
    PTN_PIN1  -  Buzzer - Ground
    ARM_BTN1  -  Push Button to 5v (Or to an arduino with a keypad that mimics the button press time)
    ACT_PIN1  -  LED to Resistor to 5v
    SRN_PIN1  -  Relay Module to 5v and ground then Normally Open (NO) to Self Powered Siren
*/



//Region Include Librarys
#include <RCSwitch.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <FS.h>
#include <Arduino.h>

//Region Define Pins
#define RF_PIN1 D2
#define PTN_PIN1 D3
#define ARM_BTN1 D4
#define ACT_PIN1 D5
#define SRN_PIN1 D6


//Region Define ON/OFF for Pins
#define BTN_DOWN LOW
#define BTN_UP HIGH
#define PTN_OFF LOW
#define PTN_ON HIGH
#define PTN_OFF LOW
#define ACT_OFF HIGH
#define ACT_ON LOW
#define SRN_OFF LOW
#define SRN_ON HIGH

//Region Initialise Librarys
RCSwitch mySwitch = RCSwitch();
ESP8266WebServer server(80);
WiFiClient client;

//Region Global Variables
/*Wifi*/
String ssid = "internet";
String password = "Internet01";
/*RF*/
String receivedValue = "";
unsigned long lastValue = 0;
/*Timing*/
unsigned long previousMillis = 0;
unsigned long mspreviousMillis = 0;
unsigned long clockTick = 0;
unsigned long mswaitval = 0;
unsigned long buttonPressMillis = 0;
/*Arm Button Debounce Variable*/
bool ARM_ready = false;
/*Config Global Variables for Startup*/
String ntfyServerAddress = "http://ntfy.sh/icalarms";
String exitdelay = "27";
String sirendelay = "5";
/*Other*/
byte srn_delay = 2;
byte arm_delay = 0;
bool arm_disabled = false;
String morseString = "..";
bool mute = false;


void setup() {

  /*Start Serial*/
  Serial.begin(115200);
  delay(200);
  /*Set Pin Types*/
  pinMode(PTN_PIN1, OUTPUT);
  pinMode(ACT_PIN1, OUTPUT);
  pinMode(SRN_PIN1, OUTPUT);
  pinMode(ARM_BTN1, INPUT_PULLUP);
  /*Start file system*/
  Serial.println("Starting File System");
  SPIFFS.begin();
  /*Get Global Variables from Config Files*/
  Serial.println("Loading Configs");
  {
    Serial.print("Wifi SSID: ");
    String tmp = readAttr("ssid", "wifi");
    ssid = (tmp == "") ? ssid : tmp;
    Serial.println(ssid);
  }
  {
    Serial.print("Password: ");
    String tmp = readAttr("pass", "wifi");
    password = (tmp  == "") ? password : tmp;
    Serial.println(password);
  }
  {
    Serial.print("Ntfy server: ");
    String tmp = readAttr("configs", "ntfy");
    ntfyServerAddress = (tmp == "") ? ntfyServerAddress : tmp;
    Serial.println(ntfyServerAddress);
  }
  {
    Serial.print("Exit Delay: ");
    String tmp = readAttr("exit", "timing");
    exitdelay = (tmp == "") ? exitdelay : tmp;
    Serial.println(exitdelay);
  }
  {
    Serial.print("Siren Delay: ");
    String tmp = readAttr("siren", "timing");
    sirendelay = (tmp == "") ? sirendelay : tmp;
    Serial.println(sirendelay);
  }

  /*Start Wifi and Wait until Connected*/
  Serial.print("Connecting to WiFi '");
  Serial.print(ssid);
  Serial.print("' with password '");
  Serial.print(password);
  Serial.print("' ");

  WiFi.begin(ssid.c_str(), password.c_str());
  int tt = 0;
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ACT_PIN1, ((digitalRead(ACT_PIN1) == ACT_ON) ? ACT_OFF : ACT_ON));
    tt++;
    timeout++;
    if (timeout > 100) {

      timeout = 0;
      tt = 0;
      ssid = "internet";
      password = "Internet01";

      WiFi.begin(ssid.c_str(), password.c_str());
      Serial.println("");
      Serial.println("Initial Setup requires you to have a wifi network named: ");
      Serial.println(ssid);
      Serial.println("with a password: '");
      Serial.println(password);
      Serial.println("-");
      Serial.println("You will be able to change the SSID and password once you have");
      Serial.println("initially joined this network and proceeded to the IP address");
      Serial.println("provided once connected.");

    }
    if (tt > 4) {
      Serial.println("");
      tt = 0;
    }
    delay(100);

    Serial.print(".");
  }

  digitalWrite(ACT_PIN1, ACT_OFF);
  Serial.println("Success!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  const char* hostname = "icalarm";
  if (MDNS.begin(hostname)) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up mDNS responder");
  }
  
  /*Setup Webserver*/
  server.on("/", HTTP_GET, handleRoot);
  server.on("/alarm", HTTP_GET, handleAlarmOff);
  server.on("/config", HTTP_POST, handleConfig);
  server.on("/delete", HTTP_POST, handleDelete);
  server.on("/linkdevice", HTTP_GET, handleLinkDevice);
  server.on("/ntfy", HTTP_POST, handleNtfy);
  server.on("/reset", HTTP_POST, handleReset);
  server.on("/wipe", HTTP_GET, handleWipe);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/timing", HTTP_POST, handleTiming);
  server.on("/wifi", HTTP_POST, handleWifi);
  server.begin();

  /*Enable RF Recieve*/
  Serial.println("Starting Radio Frequency Reciever");
  mySwitch.enableReceive(RF_PIN1);

  Serial.println("Ready!");

  ARM_ready = true;
}

void loop() {
  unsigned long currentMillis = millis();

  if ((digitalRead(ARM_BTN1) == BTN_DOWN) && ARM_ready) {
    ARM_ready = false;
    buttonPressMillis = currentMillis;
    digitalWrite(ACT_PIN1, ACT_ON);
  }

  if ((digitalRead(ARM_BTN1) == BTN_UP) && !ARM_ready) {
    digitalWrite(ACT_PIN1, ACT_OFF);
    unsigned long pressLength = currentMillis - buttonPressMillis;

    byte btn = 0;
    if (pressLength > 100) ARM_ready = true; //Debounce
    if (pressLength > 100 && pressLength <= 200) btn = 1;
    if (pressLength > 200 && pressLength <= 300) btn = 2;
    if (pressLength > 300 && pressLength <= 400) btn = 3;
    if (pressLength > 400 && pressLength <= 500) btn = 4;
    if (pressLength > 500 && pressLength <= 600) btn = 5; //Might be used in the future
    if (pressLength > 600 && pressLength <= 700) btn = 6; //Might be used in the future
    if (pressLength > 700 && pressLength <= 800) btn = 7; //Might be used in the future

    switch (btn) {
      case 0:
        break;
      case 1:
        if (arm_disabled) {
          morseString = " ... ";
          arm_disabled = false;
          if (arm_delay == 0) arm_delay = exitdelay.toInt() + 1;
          if (srn_delay > 0) arm_delay = 0;
          if (digitalRead(SRN_PIN1) == SRN_ON) arm_delay = 0;
          Serial.println("Alarm Exit Started");
        }
        break;
      case 2:
        if (arm_disabled) {
          mute = !mute;
          if (mute) Serial.println("Mute");
          if (!mute) Serial.println("Un-Mute");
          if (mute) morseString = " .. ";
          if (!mute) morseString = " -. ";
        }
        break;
      case 3:
        Serial.println("Nothing");
        morseString = " - ";
        break;
      case 4:
        morseString = " .... ";
        digitalWrite(SRN_PIN1, SRN_OFF);
        Serial.println("Siren has beed Disabled");
        arm_disabled = true;
        srn_delay = 0;
        arm_delay = 0;

        break;
    }
    Serial.print("Button Pressed (");
    Serial.print(btn);
    Serial.print(") ");
    Serial.print(pressLength);
    Serial.println("ms");


  }

    /*Clears All Signals if Arming*/
  if (arm_delay > 0) {
    if (mySwitch.available()) {
      mySwitch.resetAvailable();
    }
  }
  
  /*Handle All Signals*/
  if (mySwitch.available()) {
    unsigned long value = mySwitch.getReceivedValue();

    if ((value != 0) && (value != lastValue)) {
      lastValue = value;
      receivedValue = String(value);
      currentMillis = millis();
      digitalWrite(ACT_PIN1, ACT_ON);
      String tmp = readAttr(receivedValue, "pattern");
      String tmpMorse = (tmp  == "") ? " " : tmp;
      bool tmpNtfy = string_contains(readAttr(receivedValue, "ntfy"), "on") ? true : false;

      Serial.println(tmpNtfy);
      Serial.println(readAttr(receivedValue, "ntfy"));

      bool tmpSiren = (string_contains(readAttr(receivedValue, "siren"), "on") && (!arm_disabled) && (srn_delay == 0)) ? true : false;
      String recievedName = readAttr(receivedValue, "data");

      if (recievedName != "") {
        Serial.println(recievedName);
        if (tmpMorse != "") {
          Serial.println("Adding to Morse");
          if (!mute) morseString += " " + tmpMorse + " ";
          if (mute) morseString += "     ";
        }
        if (tmpNtfy) {
          Serial.println("Sending NTFy Request");
          sendHttpPostRequest(recievedName.c_str());
        }
        if (tmpSiren)  {
          Serial.println("Siren Sounding in 15seconds");
          srn_delay = sirendelay.toInt() + 1;
        }
      } else {
        const char* myCharPointer = receivedValue.c_str();
        //sendHttpPostRequest(myCharPointer);
        Serial.println(receivedValue);
        morseString += tmpMorse;
      }
    }
    mySwitch.resetAvailable();
  }

  /*Reset the last found RF Signal after 5000ms if an event is not active*/
  if (currentMillis - previousMillis >= 5000) {
    previousMillis = currentMillis;
    if (digitalRead(ACT_PIN1) == ACT_OFF) {
      lastValue = 0;
      receivedValue = String(lastValue );
      //Serial.println("Value: " + receivedValue);
    }
  }

  /*A seccond counter that does stuff*/
  if (currentMillis - clockTick >= 1000) {
    clockTick = currentMillis;
    if (arm_delay == 1) {
      sendHttpPostRequest("Alarm Exit");
    }
    if (arm_delay > 0) {
      arm_delay = arm_delay - 1;
      morseString = ". ";
    }
    if (srn_delay > 0) {
      srn_delay = srn_delay - 1;
    }
    if (srn_delay == 1) {
      Serial.println("Siren Is Active");
      digitalWrite(SRN_PIN1, SRN_ON);
    }
  }


  if (morseString != "") {
    if (currentMillis - mspreviousMillis >= mswaitval) {
      mspreviousMillis = currentMillis;
      mswaitval = 400;
      bool temp_PTN = PTN_ON;
      if (morseString[0] == '.') mswaitval = 200;
      if (morseString[0] == '-') mswaitval = 400;
      if (morseString[0] == ' ') mswaitval = 400;
      if (morseString[0] == ' ') temp_PTN = PTN_OFF;
      if (morseString[0] == '/') mswaitval = 400;
      if (morseString[0] == '/') temp_PTN = PTN_OFF;

      digitalWrite(PTN_PIN1, temp_PTN);

      //Serial.print(morseString[0]);

      morseString = remFC(morseString);
    }
  } else {
    digitalWrite(PTN_PIN1, PTN_OFF);
    if (ARM_ready) digitalWrite(ACT_PIN1, ACT_OFF);
  }

  if (currentMillis - mspreviousMillis >= (mswaitval - 100)) {
    digitalWrite(PTN_PIN1, PTN_OFF);
  }

  server.handleClient();



}
