#include <Arduino.h>
#include "FrekvensPanel.h"
// https://github.com/platformio/platform-espressif32/issues/367
#include <Wire.h>
#include <SPI.h>
#include "Quadrant.h"
#include "NTPHandler.h"
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#define p_ena 14
#define p_data 12
#define p_clock 2
#define p_latch 0

#define p_btn1 13 // A0 (Adafruit Feather M0) - RED button (black wire)
#define p_btn2 5  // A1 (Adafruit Feather M0) - YELLOW button (white wire)
// TODO replace by config
const char *ssid = "freebox_xxxxx";
const char *password = "xxxxx";
// TODO end replace by config
FrekvensPanel panel(p_latch, p_clock, p_data);
Quadrant NorthEast(NE);
Quadrant SouthWest(SW);
Quadrant SouthEast(SE);
Quadrant NorthWest(NW);

// Local Port to Listen For UDP Packets
uint16_t localPort;
void setup()
{

  // TODO replace by final solution CONFIG.
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  NorthWest.begin(&panel, &Serial);
  NorthEast.begin(&panel, &Serial);
  SouthWest.begin(&panel, &Serial);
  SouthEast.begin(&panel, &Serial);
  //TODO configure default birghtness
  analogWrite(p_ena, 1000);
  panel.clear();


  localPort = random(1024, 65535);
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  //Set Sync Intervals
  setSyncInterval(45);
}

void digitalClockDisplay()
{
  NorthWest.draw(hour(CE.toLocal(utc, &tcr)) / 10);
  NorthEast.draw(hour(CE.toLocal(utc, &tcr)) % 10);
  SouthWest.draw(minute() / 10);
  SouthEast.draw(minute() % 10);
}

void loop()
{
  panel.scan();
  static time_t prevDisplay = 0;
  timeStatus_t ts = timeStatus();
  utc = now();

  switch (ts)
  {
  case timeNeedsSync:
  case timeSet:
    //update the display only if time has changed
    if (now() != prevDisplay)
    {
      prevDisplay = now();
      digitalClockDisplay();
      tmElements_t tm;
      breakTime(now(), tm);
      if (ts == timeNeedsSync)
      {
        // TODO handle need of sync
      }
    }
    break;
  case timeNotSet:
    // TODO display <No Sync> If Time Not Displayed
    Serial.println("/!\\Time NOT SET /!\\");
    now();
    delay(3000);
  }
}
