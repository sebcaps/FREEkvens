#include "Quadrant.h"
#define speed 100

Coord zeroArray[4] = {{1, 1, 5, 1}, {1, 2, 1, 6}, {5, 2, 5, 6}, {1, 7, 5, 7}};
Coord oneArray[2] = {{3, 1, 4, 1}, {4, 2, 4, 7}};
Coord twoArray[5] = {{1, 1, 5, 1}, {5, 2, 5, 3}, {1, 4, 5, 4}, {1, 5, 1, 6}, {1, 7, 5, 7}};
Coord threeArray[5] = {{1, 1, 5, 1}, {5, 2, 5, 3}, {3, 4, 5, 4}, {5, 5, 5, 6}, {1, 7, 5, 7}};
Coord fourArray[3] = {{1, 1, 1, 4}, {2, 4, 4, 4}, {5, 1, 5, 7}};
Coord fiveArray[5] = {{1, 1, 1, 5}, {1, 2, 1, 3}, {1, 4, 5, 4}, {5, 5, 5, 6}, {1, 7, 5, 7}};
Coord sixArray[4] = {{1, 1, 5, 1}, {1, 2, 1, 7}, {2, 4, 5, 4}, {2, 7, 5, 7}};
Coord sevenArray[2]{{1, 1, 5, 1}, {5, 2, 5, 7}};
Coord eightArray[5] = {{1, 1, 5, 1}, {1, 1, 1, 7}, {5, 1, 5, 7}, {2, 4, 4, 4}, {2, 7, 4, 7}};
Coord nineArray[5] = {{1, 1, 5, 1}, {1, 4, 5, 4}, {1, 7, 5, 7}, {1, 2, 1, 3}, {5, 2, 5, 7}};

Number zero(zeroArray, 4);
Number one(oneArray, 2);
Number two(twoArray, 5);
Number three(threeArray, 5);
Number four(fourArray, 3);
Number five(fiveArray, 5);
Number six(sixArray, 4);
Number seven(sevenArray, 2);
Number eight(eightArray, 5);
Number nine(nineArray, 5);

//struct NewNumber zeroS = {4, zeroArray};
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
  
  _stream->println(_currentNombre);
}

void Quadrant::draw(byte nombre)
{
  _stream->println("In draw");
  _stream->println(_currentNombre);
  _stream->println(_quad);
  if (_currentNombre == 99)
  { // first time a number is displayed in quadrant
    for (int i = 0; i < _availableNumber[nombre]._nbreLigne; i++)
    {
      _panel->drawLine(_availableNumber[nombre]._coordo[i].x1 + _x, _availableNumber[nombre]._coordo[i].y1 + _y, _availableNumber[nombre]._coordo[i].x2 + _x, _availableNumber[nombre]._coordo[i].y2 + _y, 1);
    }
    _currentNombre = nombre;
  }
  else if (nombre != _currentNombre)
  {
    //changeNumber
    _stream->println("NuMBER CHANGES");
    changeNumber(nombre);
  }
  else
  {
    //NOPE

    _panel->scan();
    _stream->println("NO CHANGES");
  }
}
void Quadrant::changeNumber(byte nombre)
{
  switch (_quad)
  {
  case NW:
    ScrollLeft(_currentNombre);
    ScrollDown(nombre);
    break;
  case NE:
    ScrollRight(_currentNombre);
    ScrollDown(nombre);
    break;
  case SW:
    ScrollLeft(_currentNombre);
    ScrollUp(nombre);
    break;
  case SE:
    ScrollRight(_currentNombre);
    ScrollUp(nombre);
    break;
  }
  _currentNombre = nombre;
}
//// useful only for NW a SW, not checked with others
void Quadrant::ScrollLeft(byte nombre)
{
  //  //    Number myNumb = _availableNumber[nombre];
  int x = 0;
  while (x < 8)
  {
    for (int i = 0; i < _availableNumber[nombre]._nbreLigne; i++)
    {
      _panel->drawLine(_availableNumber[nombre]._coordo[i].x1 + _x - x, _availableNumber[nombre]._coordo[i].y1 + _y, _availableNumber[nombre]._coordo[i].x2 + _x - x, _availableNumber[nombre]._coordo[i].y2 + _y, 1);
    }
    delay(speed);
    x++;
    _panel->scan();
    _panel->fillRect(_x, _y, 8, 8, 0);
  }
}
//// useful only for NW a NE, not checked with others
void Quadrant::ScrollDown(byte nombre)
{
  int x = 0;
  while (x < 8)
  {
    for (int i = 0; i < _availableNumber[nombre]._nbreLigne; i++)
    {
      _panel->drawLine(_availableNumber[nombre]._coordo[i].x1 + _x, _availableNumber[nombre]._coordo[i].y1 + _y + x - 7, _availableNumber[nombre]._coordo[i].x2 + _x, _availableNumber[nombre]._coordo[i].y2 + _y + x - 7, 1);
    }
    delay(speed);
    x++;
    _panel->scan();
    _panel->fillRect(_x, _y, 8, 8, 0);
  }
}
//// useful only for NW a NE, not checked with others
void Quadrant::ScrollUp(byte nombre)
{
  int x = 0;
  while (x < 8)
  {
    for (int i = 0; i < _availableNumber[nombre]._nbreLigne; i++)
    {
      _panel->drawLine(_availableNumber[nombre]._coordo[i].x1 + _x, _availableNumber[nombre]._coordo[i].y1 + _y - x + 7, _availableNumber[nombre]._coordo[i].x2 + _x, _availableNumber[nombre]._coordo[i].y2 + _y - x + 7, 1);
    }
    delay(speed);
    x++;
    _panel->scan();
    _panel->fillRect(_x, _y, 8, 8, 0);
  }
}
// useful only for NE a SE, not checked with others
void Quadrant::ScrollRight(byte nombre)
{
  int x = 0;
  while (x < 8)
  {
    for (int i = 0; i < _availableNumber[nombre]._nbreLigne; i++)
    {
      _panel->drawLine(_availableNumber[nombre]._coordo[i].x1 + _x + x, _availableNumber[nombre]._coordo[i].y1 + _y, _availableNumber[nombre]._coordo[i].x2 + _x + x, _availableNumber[nombre]._coordo[i].y2 + _y, 1);
    }
    delay(speed);
    x++;
    _panel->scan();
    _panel->fillRect(_x, _y, 8, 8, 0);
  }
}
//void Quadrant:: moveNumber(sens direction, axis axe) {
//}
