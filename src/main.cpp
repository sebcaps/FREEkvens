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
#include "button.h"

#define p_ena 14
#define p_data 12
#define p_clock 2
#define p_latch 0

#define p_btn1 13 // A0 (Adafruit Feather M0) - RED button (black wire)
#define p_btn2 5  // A1 (Adafruit Feather M0) - YELLOW button (white wire)
// TODO replace by config
const char *ssid = "freebox_XZQPTI";
const char *password = "julie0807";

extern struct ConfigSettingsStruct ConfigSettings;
extern struct ConfigPanel cfgPanel;
enum
{
  HOUR,
  TEMP,
  HOURTEMP
} program;

bool configOK = false;
String modeWiFi = "STA";

FrekvensPanel panel(p_latch, p_clock, p_data);
byte activeProgram = TEMP;
byte activeBrightMode = 1;
// TODO set to 0
byte currentTemp = 21;

SimpleButton button1(1, p_btn1, 1, 20);
SimpleButton button2(2, p_btn2, 1, 20);

Quadrant NorthEast(NE);
Quadrant SouthWest(SW);
Quadrant SouthEast(SE);
Quadrant NorthWest(NW);

// Local Port to Listen For UDP Packets
uint16_t localPort;
void setProgram(int program)
{
  activeProgram = program % 3;
}
void buttonHandler(int id, int state)
{
  if (state) // button pressed
  {
    switch (id)
    {
    case 1:
      // TODO handle button to manage brigthness ?
      // setBrightMode(activeBrightMode + 1);
      break;
    case 2:
      // FIXME is this the correct location to clear panel ??
      panel.clear();
      setProgram(activeProgram + 1);
      break;
    }
  }
}
// TODO return Temp from sensor
byte getTemp()
{
  return 20;
}
void setup()
{

  // TODO replace by final solution CONFIG.
  Serial.begin(115200);
  // Input for buttons
  pinMode(p_btn1, INPUT_PULLUP);
  pinMode(p_btn2, INPUT_PULLUP);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  button1.attachHandler(buttonHandler);
  button2.attachHandler(buttonHandler);
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
  // Initialise Temp;
  currentTemp = getTemp();
  // Initialise panel Quadrants
  NorthWest.begin(&panel, &Serial);
  NorthEast.begin(&panel, &Serial);
  SouthWest.begin(&panel, &Serial);
  SouthEast.begin(&panel, &Serial);
  //TODO configure default brightness
  analogWrite(p_ena, 1000);
  panel.clear();

  localPort = random(1024, 65535);
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  //Set Sync Intervals
  setSyncInterval(45);
}
void displayTemp(byte temp)
{
  panel.clear();
  panel.setTextSize(0);
  panel.setCursor(2, 4);
  panel.print(temp);
  panel.drawCircle(14, 3, 1, 1);
  // panel.setCursor()
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
  byte temp;
  panel.scan();
  static time_t prevDisplay = 0;
  timeStatus_t ts = timeStatus();
  utc = now();
  button1.scan();
  button2.scan();
  switch (activeProgram)
  {
  case HOUR:
    Serial.println("Just display HOUR...");
    Serial.println(activeProgram);
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
    break;
  case TEMP:
    Serial.println("Will display TEMP...");
    temp = getTemp();
    // if (temp != currentTemp)
    // {
    currentTemp = temp;
    displayTemp(currentTemp);
    // }

    break;
  case HOURTEMP:
    Serial.println("Will display HOUR & TEMP...");
    break;
  }
}
