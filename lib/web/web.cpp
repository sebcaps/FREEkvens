#include <Arduino.h>
#include "ArduinoJson.h"
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "web.h"
#include "config.h"

extern struct WifiConfigStruct wifiSettings;
extern struct TimeConfigStruct timeConfig;

String TimeZone = "";
String NTPServer = "";

ESP8266WebServer serverWeb(80);

void webServerHandleClient()
{
  serverWeb.handleClient();
}

void initWebServer()
{
  Serial.println("In initWebServer");
  serverWeb.serveStatic("/js/jquery-min.js", LittleFS, "/web/js/jquery-min.js");
  serverWeb.serveStatic("/js/functions.js", LittleFS, "/web/js/functions.js");
  serverWeb.serveStatic("/js/bootstrap.min.js", LittleFS, "/web/js/bootstrap.min.js");
  serverWeb.serveStatic("/js/bootstrap.min.js.map", LittleFS, "/web/js/bootstrap.min.js.map");
  serverWeb.serveStatic("/css/bootstrap.min.css", LittleFS, "/web/css/bootstrap.min.css");
  serverWeb.serveStatic("/css/style.css", LittleFS, "/web/css/style.css");
  serverWeb.serveStatic("/img/logo.png", LittleFS, "/web/img/logo.png");
  serverWeb.serveStatic("/web/img/wait.gif", LittleFS, "/web/img/wait.gif");
  serverWeb.serveStatic("/web/img/", LittleFS, "/web/img/");
  serverWeb.serveStatic("/", LittleFS, "/web/html/index.html");

  serverWeb.on("/saveWIFI", HTTP_POST, handleSaveWifi);
  serverWeb.on("/saveTime", HTTP_POST, handleSaveTime);
  serverWeb.on("/saveOth", HTTP_POST, handleSaveOther);
  serverWeb.on("/api/time/", HTTP_GET, handleTimeAPI);
  serverWeb.on("/api/wifi/", HTTP_GET, handleWifiAPI);
  serverWeb.on("/api/other/", HTTP_GET, handleOthAPI);
  serverWeb.on("/reboot", handleReboot);
  serverWeb.onNotFound(handleNotFound);
  serverWeb.begin();
}

void handleNotFound()
{

  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += serverWeb.uri();
  message += F("\nMethod: ");
  message += (serverWeb.method() == HTTP_GET) ? "GET" : "POST";
  message += F("\nArguments: ");
  message += serverWeb.args();
  message += F("\n");

  for (uint8_t i = 0; i < serverWeb.args(); i++)
  {
    message += " " + serverWeb.argName(i) + ": " + serverWeb.arg(i) + "\n";
  }

  serverWeb.send(404, F("text/plain"), message);
}

void handleWifiAPI()
{
  File wifiFile = LittleFS.open("/config/wifi.json", "r");
  serverWeb.streamFile(wifiFile, "text/json");
  wifiFile.close();
}
void handleOthAPI()
{
  File confFile = LittleFS.open("/config/other.json", "r");
  serverWeb.streamFile(confFile, "text/json");
  confFile.close();
}
void handleTimeAPI()
{
  File timeFile = LittleFS.open("/config/time.json", "r");
  serverWeb.streamFile(timeFile, "text/json");
  timeFile.close();
}

void handleSaveTime()
{
  if (!serverWeb.hasArg("selectTZ"))
  {
    serverWeb.send(500, "text/plain", "BAD ARGS");
    return;
  }
  const int capacity = JSON_OBJECT_SIZE(2);
  StaticJsonDocument<capacity> doc;
  doc["TimeZone"] = serverWeb.arg("selectTZ");
  doc["NTPServer"] = serverWeb.arg("selectNTP");
  File timeFile = LittleFS.open("/config/time.json", "w");
  if (!timeFile)
  {
  }
  else
  {
    serializeJson(doc, timeFile);
  }
  serverWeb.sendHeader(F("Location"), F("/"));
  serverWeb.send(303);
}

void handleSaveWifi()
{
  if (!serverWeb.hasArg("WIFISSID"))
  {
    serverWeb.send(500, "text/plain", "BAD ARGS");
    return;
  }
  const int capacity = JSON_OBJECT_SIZE(6) + 100;
  StaticJsonDocument<capacity> doc;
  doc["ssid"] = serverWeb.arg("WIFISSID");
  doc["pass"] = serverWeb.arg("WIFIpassword");
  doc["ip"] = serverWeb.arg("ipAddress");
  doc["mask"] = serverWeb.arg("ipMask");
  doc["gw"] = serverWeb.arg("ipGW");
  doc["dns"] = serverWeb.arg("dnsName");
  File configFile = LittleFS.open("/config/wifi.json", "w");
  if (!configFile)
  {
  }
  else
  {
    serializeJson(doc, configFile);
  }
  serverWeb.send(200, "text/html", "Save config OK ! <br><form method='GET' action='reboot'><input type='submit' name='reboot' value='Reboot'></form>");
}
void handleSaveOther()
{
  if (!serverWeb.hasArg("defaultStart"))
  {
    serverWeb.send(500, "text/plain", "BAD ARGS");
    return;
  }
  const int capacity = JSON_OBJECT_SIZE(4) + 70;
  StaticJsonDocument<capacity> doc;
  doc["StartMode"] = serverWeb.arg("defaultStart");
  doc["EnableDynamicBright"] = serverWeb.hasArg("enableDynamic") ? "1" : "0";
  doc["MaxBright"] = serverWeb.hasArg("maxBright") ? serverWeb.arg("maxBright") : "";
  doc["StaticBrightness"] = serverWeb.hasArg("staticBright") ? serverWeb.arg("staticBright") : "";

  File otherConfigFile = LittleFS.open("/config/other.json", "w");
  if (!otherConfigFile)
  {
  }
  else
  {
    // json.printTo(configFile);
    serializeJson(doc, otherConfigFile);
  }
  serverWeb.send(200, "text/html", "Save config OK ! <br><form method='GET' action='reboot'><input type='submit' name='reboot' value='Reboot'></form>");
}
// void handleTools()
// {
//   String result;

//   result += F("<html>");
//   result += FPSTR(HTTP_HEADER);
//   result += F("<h1>Tools</h1>");
//   result += F("<div class='btn-group-vertical'>");
//   result += F("<a href='/reboot' class='btn btn-primary mb-2'>Reboot</button>");
//   result += F("</div></body></html>");

//   serverWeb.send(200, F("text/html"), result);
// }

void handleReboot()
{
  String result;

  result += F("<html>");
  // result += FPSTR(HTTP_HEADER);
  result += F("<h1>Reboot ...</h1>");
  result = result + F("</body></html>");
  serverWeb.sendHeader(F("Location"), F("/"));
  serverWeb.send(303);

  ESP.restart();
}

void handleUpdate()
{
  String result;

  result += F("<html>");
  // result += FPSTR(HTTP_HEADER);
  result += F("<h1>Update ...</h1>");
  result += F("<div class='row justify-content-md-center' >");
  result += F("<div class='col-sm-6'><form method='POST' action='StartOTA' enctype='multipart/form-data'>");
  result += F("<div class='form-group'>");
  result += F(" <label for='ip'>File : </label>");
  result += F(" <input type='file' name='file' class='form-control' id='file' />");
  result += F("</div>");
  result += F("<button type='submit' class='btn btn-primary mb-2'>Update</button>");
  result = result + F("</form></body></html>");

  serverWeb.send(200, F("text/html"), result);
}

// void handleFSbrowser()
// {
//   String result;
//   result += F("<html>");
//   result += FPSTR(HTTP_HEADER);
//   result += F("<h1>FSBrowser</h1>");
//   result += F("<nav id='navbar-custom' class='navbar navbar-default navbar-fixed-left'>");
//   result += F("      <div class='navbar-header'>");
//   result += F("        <!--<a class='navbar-brand' href='#'>Brand</a>-->");
//   result += F("      </div>");
//   result += F("<ul class='nav navbar-nav'>");

//   String str = "";
//   Dir dir = LittleFS.openDir("/config/");
//   while (dir.next())
//   {
//     String tmp = dir.fileName();
//     tmp = tmp.substring(8);
//     result += F("<li><a href='#' onClick=\"readfile('");
//     result += tmp;
//     result += F("');\">");
//     result += tmp;
//     result += F(" ( ");
//     result += dir.fileSize();
//     result += F(" o)</a></li>");
//   }
//   result += F("</ul></nav>");
//   result += F("<div class='container-fluid' >");
//   result += F("  <div class='app-main-content'>");
//   result += F("<form method='POST' action='saveFile'>");
//   result += F("<div class='form-group'>");
//   result += F(" <label for='file'>File : <span id='title'></span></label>");
//   result += F("<input type='hidden' name='filename' id='filename' value=''>");
//   result += F(" <textarea class='form-control' id='file' name='file' rows='10'>");
//   result += F("</textarea>");
//   result += F("</div>");
//   result += F("<button type='submit' class='btn btn-primary mb-2'>Save</button>");
//   result += F("</Form>");
//   result += F("</div>");
//   result += F("</div>");
//   result += F("</body></html>");

//   serverWeb.send(200, F("text/html"), result);
// }

void handleReadfile()
{
  String result;
  String filename = "/config/" + serverWeb.arg(0);
  File file = LittleFS.open(filename, "r");

  if (!file)
  {
    return;
  }

  while (file.available())
  {
    result += (char)file.read();
  }
  file.close();
  serverWeb.send(200, F("text/html"), result);
}
