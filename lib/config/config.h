#ifndef __CONFIG_H
#define __CONFIG_H 
#include <Arduino.h>
#include "LittleFS.h"
#include <ESP8266WiFi.h>
#include "ArduinoJson.h"

struct WifiConfigStruct
{
    String ssid;
    String password;
    String ipAddress;
    String ipMask;
    String ipGW;
    String dnsName;
};

struct TimeConfigStruct
{
    byte TimeZone;
    byte NTPServer;
};

struct OtherConfigStruct
{
    byte dynamicBrigth;
    int maxBrigth;
    int staticBright;
    byte startMode;
};

extern struct WifiConfigStruct wifiSettings;
extern struct TimeConfigStruct timeConfig;
extern struct OtherConfigStruct otherConfig;

IPAddress parse_ip_address(const char *str);
bool loadWifiConfig();
bool loadTimeConfig();
bool loadOtherConfig();

#endif