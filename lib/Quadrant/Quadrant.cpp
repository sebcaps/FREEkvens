#include "Quadrant.h"
#define speed 100

Coord zeroArray[4] = {{1, 1, 5, 1}, {1, 2, 1, 6}, {5, 2, 5, 6}, {1, 7, 5, 7}};
Coord oneArray[2] = {{3, 1, 5, 1}, {5, 2, 5, 7}};
Coord twoArray[5] = {{1, 1, 5, 1}, {5, 2, 5, 3}, {1, 4, 5, 4}, {1, 5, 1, 6}, {1, 7, 5, 7}};
Coord threeArray[5] = {{1, 1, 5, 1}, {5, 2, 5, 3}, {3, 4, 5, 4}, {5, 5, 5, 6}, {1, 7, 5, 7}};
Coord fourArray[3] = {{1, 1, 1, 4}, {2, 4, 4, 4}, {5, 1, 5, 7}};
Coord fiveArray[5] = {{1, 1, 5, 1}, {1, 2, 1, 3}, {1, 4, 5, 4}, {5, 5, 5, 6}, {1, 7, 5, 7}};
Coord sixArray[5] = {{1, 1, 5, 1}, {1, 2, 1, 7}, {2, 4, 5, 4}, {2, 7, 5, 7}, {5, 5, 5, 6}};
Coord sevenArray[2]{{1, 1, 5, 1}, {5, 2, 5, 7}};
Coord eightArray[5] = {{1, 1, 5, 1}, {1, 1, 1, 7}, {5, 1, 5, 7}, {2, 4, 4, 4}, {2, 7, 4, 7}};
Coord nineArray[5] = {{1, 1, 5, 1}, {1, 4, 5, 4}, {1, 7, 5, 7}, {1, 2, 1, 3}, {5, 2, 5, 7}};

Number zero(zeroArray, 4);
Number one(oneArray, 2);
Number two(twoArray, 5);
Number three(threeArray, 5);
Number four(fourArray, 3);
Number five(fiveArray, 5);
Number six(sixArray, 5);
Number seven(sevenArray, 2);
Number eight(eightArray, 5);
Number nine(nineArray, 5);

Quadrant::Quadrant(location pos)
{
  _quad = pos;
  _currentNombre = 99;
  switch (pos)
  {
  case NE:
    _x = 8;
    _y = 0;
    break;
  case SW:
    _x = 0;
    _y = 8;
    break;
  case SE:
    _x = 8;
    _y = 8;
    break;
  case NW:
    _x = 0;
    _y = 0;
    break;
  }
}

void Quadrant::begin(FrekvensPanel *panel, HardwareSerial *serial)
{
  _panel = panel;
  _stream = serial;
  zero.begin(panel);
  one.begin(panel);
  two.begin(panel);
  three.begin(panel);
  four.begin(panel);
  five.begin(panel);
  six.begin(panel);
  seven.begin(panel);
  eight.begin(panel);
  nine.begin(panel);
  _availableNumber[0] = zero;
  _availableNumber[1] = one;
  _availableNumber[2] = two;
  _availableNumber[3] = three;
  _availableNumber[4] = four;
  _availableNumber[5] = five;
  _availableNumber[6] = six;
  _availableNumber[7] = seven;
  _availableNumber[8] = eight;
  _availableNumber[9] = nine;
}

//void Quadrant:: moveNumber(sens direction, axis axe) {
//}

void Quadrant::draw(byte nombre, boolean forceDisplay)
{
  if (_currentNombre == 99 || forceDisplay)
  { // first time a number is displayed in quadrant or need to redraw since force=true
    // just draw it without animation
    _stream->print("In draw, with force =");
    _stream->println(forceDisplay);
    _panel->fillRect(_x, _y, 8, 8, 0);
    _availableNumber[nombre].draw(_x, _y);
    _currentNombre = nombre;
    _panel->scan();
  }
  // new number need to be updated, handle animation
  else if (nombre != _currentNombre)
  {
    changeNumber(nombre);
    _panel->scan();
  }
  else
  {
    //Just update screen
    _panel->scan();
  }
}
void Quadrant::changeNumber(byte nombre)
{
  switch (_quad)
  {
  case NW:
    _availableNumber[_currentNombre].scrollLeft(_x, _y);
    _availableNumber[nombre].scrollDown(_x, _y);
    break;
  case NE:
    _availableNumber[_currentNombre].scrollRight(_x, _y);
    _availableNumber[nombre].scrollDown(_x, _y);
    break;
  case SW:
    _availableNumber[_currentNombre].scrollLeft(_x, _y);
    _availableNumber[nombre].scrollUp(_x, _y);
    break;
  case SE:
    _availableNumber[_currentNombre].scrollRight(_x, _y);
    _availableNumber[nombre].scrollUp(_x, _y);
    break;
  }
  _currentNombre = nombre;
}
