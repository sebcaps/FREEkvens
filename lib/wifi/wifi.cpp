#include "wifi.h"

void setupWifiAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  Serial.println("In setupWifiAP");
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "IKEA-" + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  String WIFIPASSSTR = "admin" + macID;
  char WIFIPASS[WIFIPASSSTR.length() + 1];
  memset(WIFIPASS, 0, WIFIPASSSTR.length() + 1);
  for (int i = 0; i < WIFIPASSSTR.length(); i++)
    WIFIPASS[i] = WIFIPASSSTR.charAt(i);

  WiFi.softAP(AP_NameChar, WIFIPASS);
  Serial.println("In AP MODE");
  Serial.println(WiFi.softAPIP());
}

bool setupSTAWifi()
{

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(wifiSettings.ssid.c_str(), wifiSettings.password.c_str());

  IPAddress ip_address = parse_ip_address(wifiSettings.ipAddress.c_str());
  IPAddress gateway_address = parse_ip_address(wifiSettings.ipGW.c_str());
  IPAddress netmask = parse_ip_address(wifiSettings.ipMask.c_str());

  WiFi.config(ip_address, gateway_address, netmask);
  // TODO verify not empty
  if (wifiSettings.dnsName != "")
  {
    WiFi.hostname(wifiSettings.dnsName);
    ArduinoOTA.setHostname(wifiSettings.dnsName.c_str());
    MDNS.begin(wifiSettings.dnsName);
  }
  int countDelay = 50;
  while (WiFi.status() != WL_CONNECTED)
  {
#ifdef USE_WDT
    wdt_reset();
#endif
    countDelay--;
    if (countDelay == 0)
    {
      return false;
    }
    delay(250);
  }
  Serial.println("In setupSTAWifi MODE");
  WiFi.printDiag(Serial);
  return true;
}
