void handleSave() {
  String name = server.arg("deviceName");
  String deviceID = server.arg("deviceID");
  // Use the received value as the filename
  String filename = "/data_" + deviceID + ".txt";

  // Save the received value and name to the specified filename in SPIFFS
  File dataFile = SPIFFS.open(filename, "w");
  if (dataFile) {
    dataFile.println(name);
    dataFile.close();
  }

  // Redirect to the /linkdevice page
  server.sendHeader("Location", "/linkdevice");
  server.send(303);
}
