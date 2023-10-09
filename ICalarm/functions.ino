String stringReplace(String original, String find, String replace) {
  int startPos = 0;
  while ((startPos = original.indexOf(find, startPos)) != -1) {
    original = original.substring(0, startPos) + replace + original.substring(startPos + find.length());
    startPos += replace.length();
  }
  return original;
}
bool string_contains(const String& haystack, const String& needle) {
  return haystack.indexOf(needle) != -1;
}
String remFC(String str) {
  if (str == NULL || str[0] == '\0') {
    // The string is empty or null, no need to remove a character
    return "";
  }

  // Start shifting characters to the left
  String nstr = "";
  for (int i = 1; str[i] != '\0'; i++) {
    nstr += str[i];
  }
  return nstr;
}

String generateRandomString() {
  char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  String randomString = "";

  for (int i = 0; i < 6; i++) {
    int index = random(strlen(charset));
    randomString += charset[index];
  }

  return randomString;
}

String readAttr(const String settingsName, const String group) {
  String filename = "/" + group + "_" + settingsName + ".txt";
  String tempLine = "";
  if (SPIFFS.exists(filename)) {
    File file = SPIFFS.open(filename, "r");
    tempLine += file.readStringUntil('\n');
    file.close();
  }
  tempLine  = stringReplace(tempLine, "\n", "");
  tempLine.trim();
  return tempLine;
}

void writeAttr(const String settingsName, const String group, const String value) {
  String filename = "/" + group + "_" + settingsName + ".txt";
  // Save the received value and name to the specified filename in SPIFFS
  File dataFile = SPIFFS.open(filename, "w");
  if (dataFile) {
    dataFile.println(value);
    dataFile.close();
  }
  return;
}

void sendHttpPostRequest(const char* zone) {
  HTTPClient http;
  Serial.println(ntfyServerAddress);
  ntfyServerAddress.trim();
  const char* tmp  = ntfyServerAddress.c_str();
  http.begin(client, tmp);
  http.addHeader("Content-Type", "text/plain");
  http.addHeader("Title", "Home Alarm");
  http.addHeader("Priority", "urgent");
  http.addHeader("Tags", "alarm,triggered");
  int httpCode = http.POST(zone);

  if (httpCode > 0) {
    Serial.print(zone);
    Serial.print(http.getString());
    Serial.println(": Notfy Success");
  } else {
    Serial.print(zone);
    Serial.println(": Notify Failed");
  }
  http.end();
}
