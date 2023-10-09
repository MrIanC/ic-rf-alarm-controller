String htmlHeader() {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += " <head>";
  html += "   <title>IC Alarm</title>";
  html += "   <meta charset='utf-8'>";
  html += "   <link href='https://fonts.googleapis.com/icon?family=Material+Icons' rel='stylesheet'>";
  html += "   <link href='https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC' crossorigin='anonymous'>";
  html += "   <meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'>";
  html += "   <meta name='theme-color' content='#6C757D'>";
  html += " </head>";
  html += "<script src='https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js' integrity='sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM' crossorigin='anonymous'></script>";
  html += " <body>";
  return html;
}

String htmlFooter() {
  String html = "";
  html += "</body>";
  html += "</html>";
  return html;
}

String htmlNavigation() {
  String rndstr = generateRandomString();
  String html = "";
  html += "<nav class='navbar navbar-expand-lg navbar-dark bg-secondary sticky sticky-top'>";
  html += "  <div class='container-fluid'>";
  html += "    <a class='navbar-brand' href='/'>IC-Alarm</a>";
  html += "    <button class='navbar-toggler' type='button' data-bs-toggle='collapse' data-bs-target='#" + rndstr + "' aria-controls='" + rndstr + "' aria-expanded='false' aria-label='Toggle navigation'>";
  html += "      <span class='navbar-toggler-icon'></span>";
  html += "    </button>";
  html += "    <div class='collapse navbar-collapse' id='" + rndstr + "'>";
  html += "      <ul class='navbar-nav'>";
  html += "        <li class='nav-item'>";
  html += "          <a class='nav-link active' aria-current='page' href='/'>Home</a>";
  html += "        </li>";
  html += "        <li class='nav-item'>";
  html += "          <a class='nav-link active' href='/#devices'>Devices</a>";
  html += "        </li>";
  html += "        <li class='nav-item'>";
  html += "          <a class='nav-link active' href='/#ntfy'>Ntfy</a>";
  html += "        </li>";
  html += "        <li class='nav-item'>";
  html += "          <a class='nav-link active' href='/#timing'>Timing</a>";
  html += "        </li>";
  html += "        <li class='nav-item'>";
  html += "          <a class='nav-link active' href='/#wifi'>Wifi</a>";
  html += "        </li>";
  html += "        <li class='nav-item'>";
  html += "          <a class='nav-link active' href='/alarm'>Disarm</a>";
  html += "        </li>";
  html += "      </ul>";
  html += "    </div>";
  html += "  </div>";
  html += "</nav>";
  return html;
}

String htmlForm(String action, String submitText, String contents) {
  String html = "<form class='m-3' method='POST' action='/" + action + "'>";
  html += contents;
  html += "<div class='text-end'><input class='btn btn-secondary' type='submit' value='" + submitText + "'></div>";
  html += "</form>";
  return html;
}

String htmlInput(String name, String value, String label, String placeholder) {
  String rndstr = generateRandomString();
  String html = "<div class='form-floating mb-3'>";
  html += "<input type='text' class='form-control' name='" + name + "' id='" + rndstr + "' value='" + value + "' " + ((placeholder == "") ? "" : "placeholder='" + placeholder + "'") + ">";
  html += "<label for='" + rndstr + "'>" + label + "</label>";
  html += "</div>";
  return html;
}


String htmlHidden(String name, String value) {
  return "<input type='hidden' name='" + name + "' value='" + value + "'>";
}

String htmlCheck(String name, String checked, String label) {

  String rndstr = generateRandomString();
  String html = checked + "<div class='form-check form-switch'>";
  html += "  <input class='form-check-input' name='" + name + "' type='checkbox' id='" + rndstr + "' " + checked + ">";
  html += "  <label class='form-check-label' for='" + rndstr + "'>" + label + "</label>";
  html += "</div>";
  return html;
}

String htmlLink(String href, String label) {
  String html = "<div class='form-floating mb-3'>";
  html += "<div class='mb-3 mt-2 text-end'><a href='/" + href + "' class='btn btn-secondary'>" + label + "</a></div>";
  html += "</div>";
  return html;
}

String htmlDiv(String description) {
  String html = "<div class='form-floating mb-3'>";
  html += "<div class='mb-3 mt-2'>" + description + "</div>";
  html += "</div>";
  return html;
}

String htmlToughForm(String description, String contents) {
  String html = "";
  html += "<div class='dropdown'>";
  html += "<button class='btn btn-light dropdown-toggle' type='button' data-bs-toggle='dropdown' aria-expanded='false'>" + description + "</button>";
  html += "<ul class='dropdown-menu'>";
  html += "  <li>" + contents + "</li>";
  html += "</ul>";
  html += "</div>";
  return html;
}

String htmlSection(String heading, String description, String id, String contents) {
  String html = "";
  html += "<div id='" + id + "'class='p-5'></div>";
  html += "<div class='min-vh-100 mb-5'>";
  html += "<h2 class='display-3'>" + heading + "</h2>";
  html += "<div class='text-end small border-bottom mb-4'>" + description + "</div>";
  html += contents;
  html += "</div>";
  return html;
}
