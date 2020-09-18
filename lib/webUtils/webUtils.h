#ifndef __WEBUTILS_H
#define __WEBUTILS_H
#include <ESP8266WebServer.h>

extern const char *TimeZoneList[] PROGMEM = {"Europe_Paris", "Europe_London", "Africa_Niame", "America_New_York", "Asia_Seoul"};
extern const char *NTPServerList[] PROGMEM = {"pool.ntp.org", "asia.pool.ntp.org", "europe.pool.ntp.org", "oceania.pool.ntp.org", "south-america.pool.ntp.org"};

#endif