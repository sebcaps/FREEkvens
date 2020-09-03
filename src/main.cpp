#include <Arduino.h>
#include "FrekvensPanel.h"
// https://github.com/platformio/platform-espressif32/issues/367
#include <Wire.h>
#include <SPI.h>
#include "Quadrant.h"

#define p_ena 14
#define p_data 12
#define p_clock 2
#define p_latch 0

#define p_btn1 13 // A0 (Adafruit Feather M0) - RED button (black wire)
#define p_btn2 5  // A1 (Adafruit Feather M0) - YELLOW button (white wire)

FrekvensPanel panel(p_latch, p_clock, p_data);

Quadrant NorthEast(NE);
Quadrant SouthWest(SW);
Quadrant SouthEast(SE);
Quadrant NorthWest(NW);

void setup()
{
  Serial.begin(115200);
  NorthWest.begin(&panel, &Serial);
  NorthEast.begin(&panel, &Serial);
  SouthWest.begin(&panel, &Serial);
  SouthEast.begin(&panel, &Serial);
  analogWrite(p_ena, 1000);
  panel.clear();
}

void loop()
{
  panel.scan();
  NorthWest.draw(0);
  NorthEast.draw(0);
  SouthWest.draw(0);
  SouthEast.draw(0);
  delay(3000);
 
  for (int i = 1; i < 10; i++)
  {
    NorthWest.draw(i);
    NorthEast.draw(i);
    SouthWest.draw(i);
    SouthEast.draw(i);
    delay(1000);
  }
  delay(5000);
}
