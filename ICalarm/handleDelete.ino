void handleDelete() {

  String deviceID = server.arg("deviceID");
  String filename = "/data_" + deviceID + ".txt";

  if (SPIFFS.exists(filename)) {
    SPIFFS.remove(filename);
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    server.sendHeader("Location", "/");
    server.send(303);
  }
  // Redirect to the /linkdevice page
  server.sendHeader("Location", "/");
  server.send(303);
}
