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
#include "OneWire.h"
#include "DallasTemperature.h"
#include "temperature.h"
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include "FS.h"
#include "config.h"
#include "web.h"
#include "wifi.h"
#include <ArduinoOTA.h>
#define p_ena 14
#define p_data 12
#define p_clock 2
#define p_latch 0
#define NB_PROG 2
#define p_btn1 13 // A0 (Adafruit Feather M0) - RED button (black wire)
#define p_btn2 5  // A1 (Adafruit Feather M0) - YELLOW button (white wire)
#define p_photo 0
#define p_temp 13

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
byte activeProgram = HOUR;
boolean forceDisplay = true;
// TODO get this from config ?
byte activeBrightMode = 1;
int currentTemp, temp = 0;
/********Objects******/
/*Temp Object */
OneWire oneWire(p_temp);
DallasTemperature sensor(&oneWire);
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

/* Update brigthness every 15000ms = 15s */
static esp8266::polledTimeout::periodicMs updateBrightness(15000);
/* Update temperature every 10000ms = 10s */
static esp8266::polledTimeout::periodicMs updateTemp(10000);
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
void setup()
{
  Serial.begin(115200);
  if (!LittleFS.begin())
  {
    return;
  }
  loadConfigPanel();

  if (!loadConfig())
  {
    Serial.println("Conf not loaded ....");
  }
  else
  {
    configOK = true;
  }

  if (configOK)
  {
    Serial.println("Config OK");
    if (!setupSTAWifi())
    {
      setupWifiAP();
      modeWiFi = "AP";
    }
  }
  else
  {
    Serial.println("Conf KO set AP");
    setupWifiAP();
    modeWiFi = "AP";
  }

  initWebServer();
  // Input for buttons
  pinMode(p_btn1, INPUT_PULLUP);
  pinMode(p_btn2, INPUT_PULLUP);
  button1.attachHandler(buttonHandler);
  button2.attachHandler(buttonHandler);

  // Initialise panel Quadrants
  NorthWest.begin(&panel, &Serial);
  NorthEast.begin(&panel, &Serial);
  SouthWest.begin(&panel, &Serial);
  SouthEast.begin(&panel, &Serial);
  // Initialise temp sensor
  sensor.begin();
  // Initialise Temperature;
  temp = getTemp(sensor);

  rest = analogRead(p_photo);
  analogWrite(p_ena, computebrightness(rest));
  analogRead(p_photo);
  panel.clear();

  // TODO set as configuration VAR for MYTZ
  Serial.print("NTP Server:");
  Serial.println(cfgPanel.NTPServer);
  configTime(MYTZ, cfgPanel.NTPServer.c_str());
  // setSyncProvider(getNtpTime);
  setSyncProvider(customNtp);
  //FIX ME : is ko if no network connection...;
  // wait until we get ntp time
  // while (currentNow < 300000)
  // {
  //   currentNow = time(nullptr);
  //   Serial.println("currentNow < 300000");
  //   delay(10);
  // }
  //Set Sync Intervals
  setSyncInterval(45);
  //Initialisation for OTA
  ArduinoOTA.begin();
}
void displayTemp(int temp)
{
  panel.clear();
  panel.setTextSize(0);
  panel.setCursor(2, 4);
  Serial.print("In disp TEMP value is");
  Serial.print(temp);
  panel.print(temp);
  panel.drawCircle(14, 3, 1, 1);
}
void digitalClockDisplay()
{
  // NorthWest.draw(hour(CE.toLocal(utc, &tcr)) / 10, forceDisplay);
  // NorthEast.draw(hour(CE.toLocal(utc, &tcr)) % 10, forceDisplay);
  // SouthWest.draw(minute() / 10, forceDisplay);
  // SouthEast.draw(minute() % 10, forceDisplay);
  // Serial.println(localtime(&currentNow)->tm_hour);
  NorthWest.draw(localtime(&currentNow)->tm_hour / 10, forceDisplay);
  NorthEast.draw(localtime(&currentNow)->tm_hour % 10, forceDisplay);
  SouthWest.draw(localtime(&currentNow)->tm_min / 10, forceDisplay);
  SouthEast.draw(localtime(&currentNow)->tm_min % 10, forceDisplay);
}

void loop()
{
  webServerHandleClient();
  ArduinoOTA.handle();
  if (modeWiFi == "STA")
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      // we've lost the connection, so we need to reconnect
      setupSTAWifi();
    }
  }

  if (updateBrightness)
  {
    Serial.println("Will update brightness");
    Serial.print("Read value is :");
    Serial.println(rest);
    Serial.print("Computed value is :");
    rest = analogRead(p_photo);
    Serial.println(computebrightness(rest));
    analogWrite(p_ena, computebrightness(rest));
  }

  panel.scan();
  static time_t prevDisplay = 0;
  timeStatus_t ts = timeStatus();
  utc = now();
  currentNow = now();
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
      panel.clear();
      panel.setCursor(2, 5);
      panel.print("-- --");
      delay(3000);
    }
    forceDisplay = false;
    break;
  case TEMP:
    if (updateTemp) // if temp refresh interval elapsed
    {
      temp = getTemp(sensor);
    }
    if ((temp != currentTemp) || forceDisplay)
    {
      currentTemp = temp;
      displayTemp(temp);
    }
    else
    {
      //NOPE
    }
    forceDisplay = false;
    break;
  }
}
