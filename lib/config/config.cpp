#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "config.h"
#include "ArduinoJson.h"

ConfigSettingsStruct ConfigSettings;
ConfigPanel cfgPanel;

IPAddress parse_ip_address(const char *str)
{
  IPAddress result;
  int index = 0;

  result[0] = 0;
  while (*str)
  {
    if (isdigit((unsigned char)*str))
    {
      result[index] *= 10;
      result[index] += *str - '0';
    }
    else
    {
      index++;
      if (index < 4)
      {
        result[index] = 0;
      }
    }
    str++;
  }

  return result;
}

bool loadConfig()
{
  File configFile = LittleFS.open("/config/config.json", "r+");
    Serial.println("In load config");
  if (!configFile)
  {  Serial.println("!configFile");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024)
  {
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  configFile.readBytes(buf.get(), size);

  //StaticJsonDocument<512> jsonBuffer;
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, buf.get());

  if (doc.isNull())
  {
    return false;
  }

  char ssid_[30];
  strcpy(ssid_, doc["ssid"]);
  ConfigSettings.ssid = String(ssid_);
  char pass_[30];
  strcpy(pass_, doc["pass"]);
  ConfigSettings.password = String(pass_);
  char ip_[30];
  strcpy(ip_, doc["ip"]);
  ConfigSettings.ipAddress = String(ip_);
  char mask_[30];
  strcpy(mask_, doc["mask"]);
  ConfigSettings.ipMask = String(mask_);
  char gw_[30];
  strcpy(gw_, doc["gw"]);
  ConfigSettings.ipGW = String(gw_);

  configFile.close();
  Serial.println("********");
  Serial.println("In load config");
  Serial.println(ip_);
  Serial.println("********");
  return true;
}

bool loadConfigPanel()
{

  File panelFile = LittleFS.open("/config/panel.json", "r+");
  if (!panelFile)
  {
    return false;
  }

  size_t size = panelFile.size();
  if (size > 1024)
  {
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  panelFile.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, buf.get());

  if (doc.isNull())
  {
    return false;
  }
  Serial.println("In Load config, reading TZ & NTP");
  char tz_[512];
  strcpy(tz_, doc["TimeZone"]);
  cfgPanel.TimeZone = String(tz_);
  char ntp_[512];
  strcpy(ntp_, doc["NTPServer"]);
  cfgPanel.NTPServer = String(ntp_);
  Serial.println("//////////");
  Serial.println(tz_);
  Serial.println(ntp_);
  Serial.println("//////////");
  panelFile.close();

  return true;
}
