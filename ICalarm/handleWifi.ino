void handleWifi() {
  String tmpssid = server.arg("wifissid");
  String tmppass = server.arg("wifipass");

  writeAttr("ssid", "wifi", tmpssid);
  writeAttr("pass", "wifi", tmppass);
  
  ESP.restart();
  
  // Redirect to the /linkdevice page
  server.sendHeader("Location", "/");
  server.send(303);
}
