#ifndef __WEBUTILS_H
#define __WEBUTILS_H
#include <ESP8266WebServer.h>
#include "TZ.h"
enum Timezones
{
  Europe_Paris,
  Europe_London,
  Africa_Niame,
  America_New_York,
  Asia_Seoul
};

// extern const char *TimeZoneList[] PROGMEM = {"Europe_Paris", "Europe_London", "Africa_Niame", "America_New_York", "Asia_Seoul"};
// extern const char *NTPServerList[] = {"pool.ntp.org", "asia.pool.ntp.org", "europe.pool.ntp.org", "oceania.pool.ntp.org", "south-america.pool.ntp.org"};
extern const char *NTPServerList[5];
const char *getTimeZone(byte tz);
void getZone(byte tz);
#endif