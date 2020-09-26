#include "config.h"

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

bool loadWifiConfig()
{
  File wifiFile = LittleFS.open("/config/wifi.json", "r+");
  Serial.println("In loadWifiConfig");
  if (!wifiFile)
  {
    Serial.println("!wifiFile");
    return false;
  }
  const size_t capacity = JSON_OBJECT_SIZE(6) + 80;

  size_t size = wifiFile.size();
  if (size > 1024)
  {
    Serial.println("JSON CAPACITY ERROR !!!!");
    return false;
  }

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, wifiFile);

  if (doc.isNull())
  {
    return false;
  }

  wifiSettings.ssid = doc["ssid"].as<String>();
  wifiSettings.password = doc["pass"].as<String>();
  wifiSettings.ipAddress = doc["ip"].as<String>();
  wifiSettings.ipMask = doc["mask"].as<String>();
  wifiSettings.ipGW = doc["gw"].as<String>();
  wifiSettings.dnsName = doc["dns"].as<String>();
  wifiFile.close();

  return true;
}

bool loadTimeConfig()
{
  File timeFile = LittleFS.open("/config/time.json", "r+");
  if (!timeFile)
  {
    return false;
  }
  const size_t capacity = JSON_OBJECT_SIZE(2) + 30;
  size_t size = timeFile.size();
  if (size > capacity)
  {
    Serial.println("JSON CAPACITY ERROR !!!!");
    return false;
  }

  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, timeFile);

  timeConfig.TimeZone = doc["TimeZone"];
  timeConfig.NTPServer = doc["NTPServer"];

  timeFile.close();

  return true;
}
bool loadOtherConfig()
{

  File otherFile = LittleFS.open("/config/other.json", "r+");
  if (!otherFile)
  {

    return false;
  }
  const size_t capacity = JSON_OBJECT_SIZE(4) + 70;
  size_t size = otherFile.size();
  if (size > capacity)
  {
    Serial.println("JSON CAPACITY ERROR !!!!");
    return false;
  }
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, otherFile);

  otherConfig.dynamicBrigth = doc["EnableDynamicBright"];
  otherConfig.maxBrigth = doc["MaxBright"];
  otherConfig.staticBright = doc["StaticBrightness"];
  otherConfig.startMode = doc["StartMode"];

  otherFile.close();

  return true;
}
