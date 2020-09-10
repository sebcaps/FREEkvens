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
#include <PolledTimeout.h>
#include "brightness.h"

#define p_ena 14
#define p_data 12
#define p_clock 2
#define p_latch 0
#define NB_PROG 2
#define p_btn1 13 // A0 (Adafruit Feather M0) - RED button (black wire)
#define p_btn2 5  // A1 (Adafruit Feather M0) - YELLOW button (white wire)
#define p_photo 0
// TODO replace by config
const char *ssid = "xxxxx";
const char *password = "xxxxx";

extern struct ConfigSettingsStruct ConfigSettings;
extern struct ConfigPanel cfgPanel;
enum program
{
  HOUR,
  TEMP
};

/***********Initialise global vars & object ************/
/********Vars******/
bool configOK = false;
String modeWiFi = "STA";
// TODO get this from config
byte activeProgram = TEMP;
boolean forceDisplay = true;
// TODO get this from config ?
byte activeBrightMode = 1;
byte currentTemp = 0;
// Local Port to Listen For UDP Packets
uint16_t localPort;
/********Objects******/
/*Panel*/
FrekvensPanel panel(p_latch, p_clock, p_data);
/*Button*/
SimpleButton button1(1, p_btn1, 1, 20);
SimpleButton button2(2, p_btn2, 1, 20);
/*Quadrant (ie part of panel)*/
Quadrant NorthEast(NE);
Quadrant SouthWest(SW);
Quadrant SouthEast(SE);
Quadrant NorthWest(NW);

static esp8266::polledTimeout::periodicMs updateBrightness(15000);
int rest;

void setProgram(int program)
{
  activeProgram = program % NB_PROG;
  forceDisplay = true;
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
  // Initialise Temperature;
  currentTemp = getTemp();

  // Initialise panel Quadrants
  NorthWest.begin(&panel, &Serial);
  NorthEast.begin(&panel, &Serial);
  SouthWest.begin(&panel, &Serial);
  SouthEast.begin(&panel, &Serial);
  //TODO configure default brightness
  analogWrite(p_ena, 1000);
  analogRead(p_photo);
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
}
void digitalClockDisplay()
{
  NorthWest.draw(hour(CE.toLocal(utc, &tcr)) / 10, forceDisplay);
  NorthEast.draw(hour(CE.toLocal(utc, &tcr)) % 10, forceDisplay);
  SouthWest.draw(minute() / 10, forceDisplay);
  SouthEast.draw(minute() % 10, forceDisplay);
}

void loop()
{
  rest = analogRead(p_photo);
  if (updateBrightness)
  {
    Serial.println("Will update brightness");
    Serial.print("Read value is :");
    Serial.println(rest);
    Serial.print("Computed value is :");
    Serial.println(computebrightness(rest));
    analogWrite(p_ena,computebrightness(rest));
  }

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
  forceDisplay = false;
  break;
case TEMP:
  temp = getTemp();
  if ((temp != currentTemp) || forceDisplay)
  {
    currentTemp = temp;
    displayTemp(currentTemp);
    Serial.println("Redraw temp since it changed or force!");
  }
  else
  {
    // Serial.println("No temp changed, no redraw");
  }
  forceDisplay = false;
  break;
}
}
