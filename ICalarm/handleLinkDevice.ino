void handleLinkDevice() {
  String html = htmlHeader();
  html += htmlNavigation();
  html += "<div class='container mt-5 pb-5 pt-5'>";

  html +=  htmlSection(
             "Link", "Link a compatible 433Mhz Device", "Link",
             htmlForm(
               "save",
               "Save and Link",
               htmlInput(
                 "deviceID",
                 receivedValue,
                 "Device ID",
                 "Device ID"
               ) +
               htmlInput(
                 "deviceName",
                 "",
                 "Device Name/Area",
                 "Name Device"
               )
             ) +
             htmlDiv("If you previously had a device with the same ID, it will now be replaced with the name you've just provided.") +
             htmlLink("linkdevice", "Refresh")
           );

           html += "</div>";

  html += htmlFooter();
  server.send(200, "text/html", html);
}
