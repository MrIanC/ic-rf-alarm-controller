void handleTiming() {
  String tmpexitdelay = server.arg("exitdelay");
  String tmpsirendelay = server.arg("sirendelay");

  writeAttr("exit", "timing", tmpexitdelay);
  writeAttr("siren", "timing", tmpsirendelay);

  exitdelay = readAttr("exit", "timing");
  sirendelay = readAttr("siren", "timing");

  // Redirect to the /linkdevice page
  server.sendHeader("Location", "/");
  server.send(303);
}
