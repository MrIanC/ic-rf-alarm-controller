
void handleWipe() {
  String html = htmlHeader();
  html += htmlNavigation();
  html += "<div class='container mt-5 pb-5 pt-5'>";
  html += htmlSection(
            "Wipe Device", "Remove all Devices, NTFY settings and Timings", "wipe",
            htmlForm(
              "reset", "Reset To Defaults", ""
            )
          );
  html += "</div>";
  html += htmlFooter();
  server.send(200, "text/html", html);
}

void handleReset() {
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    String filename = dir.fileName();
    if (SPIFFS.exists(filename)) {
      SPIFFS.remove(filename);
    }
  }
  server.sendHeader("Location", "/");
  server.send(303);
}
