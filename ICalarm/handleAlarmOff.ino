void handleAlarmOff() {
  digitalWrite(SRN_PIN1, SRN_OFF);
  Serial.println("Siren has beed Disabled");
  arm_disabled = true;
  srn_delay = 0;
  arm_delay = 0;

  String html = htmlHeader();
  html += htmlNavigation();
  html += "<div class='container mt-5 pb-5 pt-5'>";
  html += htmlSection(
            "Alarm", 
            "The alarm has been Disabled", 
            "",
            "");
  html += "</div>";
  html += htmlFooter();

  server.send(200, "text/html", html);
}
