#ifndef __CONFIG_H
#define __CONFIG_H

#include <Arduino.h>
#include "LittleFS.h"

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
    String TimeZone;
    String NTPServer;
};

struct OtherConfigStruct
{
    byte dynamicBrigth;
    int maxBrigth;
    int staticBright;
    byte startMode;
};

IPAddress parse_ip_address(const char *str);
bool loadWifiConfig();
bool loadTimeConfig();
bool loadOtherConfig();

#endif