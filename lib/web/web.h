#ifndef __WEB_H
#define __WEB_H
#include "ArduinoJson.h"
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "config.h"


void initWebServer();
void webServerHandleClient();
void handleRoot();
void handleNotFound();
void handleSaveWifi();
void handleSaveTime();
void handleSaveOther();
void handleReboot();
void handleTimeAPI();
void handleWifiAPI();
void handleOthAPI();
void handlePing();

#endif