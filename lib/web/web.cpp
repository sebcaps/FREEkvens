#include "web.h"

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
  serverWeb.serveStatic("/wait", LittleFS, "/web/html/wait.html");

  serverWeb.on("/saveWIFI", HTTP_POST, handleSaveWifi);
  serverWeb.on("/saveTime", HTTP_POST, handleSaveTime);
  serverWeb.on("/saveOth", HTTP_POST, handleSaveOther);
  serverWeb.on("/api/time/", HTTP_GET, handleTimeAPI);
  serverWeb.on("/api/wifi/", HTTP_GET, handleWifiAPI);
  serverWeb.on("/api/other/", HTTP_GET, handleOthAPI);
  serverWeb.on("/ping", HTTP_GET, handlePing);
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
  const int capacity = JSON_OBJECT_SIZE(2) + 60;
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
  serverWeb.sendHeader(F("Location"), F("/wait"));
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
  serverWeb.sendHeader(F("Location"), F("/wait"));
  serverWeb.send(303);
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
    serializeJson(doc, otherConfigFile);
  }

  serverWeb.sendHeader(F("Location"), F("/wait"));
  serverWeb.send(303);
}

void handleReboot()
{
  Serial.println("/////I WILL RESTART //////////");
  ESP.restart();
}
void handlePing()
{
  serverWeb.send(200);
}
