void handleConfig() {
  String pattern = server.arg("pattern");
  String ntfy = server.arg("ntfy");
  String siren = server.arg("siren");
  String deviceID = server.arg("deviceID");

  // Use the received value as the filename
  String filename = "/pattern_" + deviceID + ".txt";
  // Save the received value and name to the specified filename in SPIFFS
  File dataFile = SPIFFS.open(filename, "w");
  if (dataFile) {
    dataFile.println(pattern);
    dataFile.close();
  }

  filename = "/ntfy_" + deviceID + ".txt";
  // Save the received value and name to the specified filename in SPIFFS
  dataFile = SPIFFS.open(filename, "w");
  if (dataFile) {
    dataFile.println(ntfy);
    dataFile.close();
  }

  filename = "/siren_" + deviceID + ".txt";
  // Save the received value and name to the specified filename in SPIFFS
  dataFile = SPIFFS.open(filename, "w");
  if (dataFile) {
    dataFile.println(siren);
    dataFile.close();
  }

  // Redirect to the /linkdevice page
  server.sendHeader("Location", "/");
  server.send(303);
}
