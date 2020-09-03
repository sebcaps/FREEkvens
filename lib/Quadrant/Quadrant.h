#ifndef __QUADRANT_H
#define __QUADRANT_H
#include "Arduino.h"
#include "FrekvensPanel.h"
#include "Number.h"

enum location
{
  NW,
  NE,
  SW,
  SE
};

struct startCoord
{
  byte x;
  byte y;
};

class Quadrant
{
public:
  Quadrant(location position);
  void begin(FrekvensPanel *panel, HardwareSerial *serial);
  void draw(byte nombre);

private:
  byte _x;
  byte _y;
  byte _currentNombre;
  Number _availableNumber[10]; // Number 0-> 9
  location _quad;
  FrekvensPanel *_panel;
  void changeNumber(byte nombre);
  void ScrollLeft(byte nombre);
  void ScrollDown(byte nombre);
  void ScrollRight(byte nombre);
  void ScrollUp(byte nombre);
  HardwareSerial *_stream;
};

#endif //__QUADRANT_H
