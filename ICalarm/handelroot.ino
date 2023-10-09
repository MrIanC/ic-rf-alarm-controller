void handleRoot() {

  String html = htmlHeader();
  html += htmlNavigation();
  html += "<div class='container mt-5 pb-5 pt-5'>";

  html += htmlSection(
            "Alarm Configuration", "", "summary",
            htmlDiv("An Effective RF Alarm System")
          );

  String deviceList = "";
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    String filename = dir.fileName();

    if (string_contains(filename, "/data_")) {
      String deviceid = stringReplace(stringReplace(filename, "/data_", ""), ".txt", "");
      String recievedName = readAttr(deviceid, "data");

      if (recievedName != "") {
        String tmpMorse = readAttr(deviceid, "pattern");
        String tmpNtfy = (string_contains(readAttr(deviceid, "ntfy"), "on") ? "checked" : "");
        String tmpSiren = (string_contains(readAttr(deviceid, "siren"), "on") ? "checked" : "");
        deviceList += "<details class='border-bottom mb-4'>";
        deviceList += "<summary class=' p-1 nav-item' style='list-style: none;'>";
        deviceList += "<span class='h4'>" + recievedName + "</span>";
        deviceList += "<span>[" + deviceid + "]</span>";
        deviceList += "</summary>";
        deviceList += "<div class='m-3'>";
        deviceList += htmlForm(
                        "config", "Save Trigger Config",
                        htmlInput("pattern", tmpMorse, "Beep Pattern", "Beep Pattern") +
                        htmlCheck("ntfy", tmpNtfy, " Notify using the ntfy Service") +
                        htmlCheck("siren", tmpSiren, " Trigger alarm when armed") +
                        htmlHidden("deviceID", deviceid)
                      );
        deviceList += htmlToughForm("Don't need the Device?", htmlForm("delete", "Delete Device", htmlHidden("deviceID", deviceid)));
        deviceList += "</div>";
        deviceList += "</details>";
      }
    }
  }

  html += htmlSection(
            "Devices", "Registered Devices", "devices",
            deviceList +
            htmlLink("linkdevice", "Link A Device")
          );

  html += htmlSection(
            "Ntfy", "Notification Service", "ntfy",
            htmlForm(
              "ntfy",
              "Save Ntfy Configuration",
              htmlInput (
                "address",
                ntfyServerAddress,
                "NTFY Server FQDN/path",
                "NTFY Server FQDN/path"
              )
            ) + htmlLink("https://ntfy.sh/", "Find out more about NTFY")
          );
  html += htmlSection(
            "Timings", "Exit and Siren delay times measured in seconds", "timing",
            htmlForm(
              "timing",
              "Save Time Configuration",
              htmlInput (
                "exitdelay",
                exitdelay,
                "Exit Delay", "Exit Delay"
              ) +
              htmlInput (
                "sirendelay",
                sirendelay,
                "Siren Delay", "Siren Delay"
              )
            )
          );
  html += htmlSection(
            "Wifi", "Set Wifi SSID and Wifi Password to connect to", "wifi",
            htmlForm(
              "wifi",
              "Save WiFi Configuration",
              htmlInput (
                "wifissid",
                ssid,
                "Wifi SSID", "Wifi SSID"
              ) +
              htmlInput (
                "wifipass",
                password,
                "Wifi Password", "Wifi Password"
              )
            )
          );

          
  html += htmlSection(
            "Reset", "Reset to defaults", "reset",
            htmlLink(
              "wipe", "Reset Device"
            )
          );

  html += "</div>";
  html += htmlFooter();
  server.send(200, "text/html", html);
}
