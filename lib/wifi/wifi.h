#ifndef __WIFI_H
#define __WIFI_H
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "config.h"

extern String modeWiFi;

void setupWifiAP();
bool setupSTAWifi();

#endif