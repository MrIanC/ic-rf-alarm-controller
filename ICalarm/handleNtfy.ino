void handleNtfy() {
  String address = server.arg("address");
  // Use the received value as the filename
  String filename = "/ntfy_configs.txt";
  // Save the received value and name to the specified filename in SPIFFS
  File dataFile = SPIFFS.open(filename, "w");
  if (dataFile) {
    dataFile.println(address);
    dataFile.close();
  }

  String tmp = readAttr("configs", "ntfy");
  ntfyServerAddress = (tmp == "") ? ntfyServerAddress : tmp;

  // Redirect to the /linkdevice page
  server.sendHeader("Location", "/");
  server.send(303);
}
