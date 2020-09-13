#ifndef __CONFIG_H
#define __CONFIG_H

#include <Arduino.h>
#include "LittleFS.h"
#define VERSION "v1.0"

struct ConfigSettingsStruct
{
    String ssid;
    String password;
    String ipAddress;
    String ipMask;
    String ipGW;
    
};

struct ConfigPanel
{
    String text;
    String sizeText;
    String scrollText;
    String xText;
    String yText;
    String light;
    
};

IPAddress parse_ip_address(const char *str);
bool loadConfig();
bool loadConfigPanel();

#endif