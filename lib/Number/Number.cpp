#include "Number.h"
//https://stackoverflow.com/questions/37074763/arduino-class-array-member/50924520

Number::Number()
{
}

Number::Number(Coord *coords, int nb_lines)
{
  _coordo = new Coord[nb_lines];
  for (int i = 0; i < nb_lines; i++)
  {
    _coordo[i] = coords[i];
  }
  _nbreLigne = nb_lines;
}
void Number::begin(FrekvensPanel *panel)
{
  _panel = panel;
}

void Number::draw(int x, int y)
{
  for (int i = 0; i < _nbreLigne; i++)
  {
    _panel->drawLine(_coordo[i].x1 + x, _coordo[i].y1 + y, _coordo[i].x2 + x, _coordo[i].y2 + y, 1);
  }
}
//// useful only for NW a SW, not checked with others
void Number::scrollLeft(byte _x, byte _y)
{
  int j = 0;
  while (j < 8)
  {
    draw(_x - j, _y);
    delay(speed);
    j++;
    _panel->scan();
    _panel->fillRect(_x, _y, 8, 8, 0);
  }
}
void Number::scrollRight(byte _x, byte _y)
{
  int j = 0;
  while (j < 8)
  {
    draw(_x + j, _y);
    delay(speed);
    j++;
    _panel->scan();
    _panel->fillRect(_x, _y, 8, 8, 0);
  }
}
//// useful only for NW a NE, not checked with others
void Number::scrollDown(byte _x, byte _y)
{
  int j = 0;
  _panel->scan();
  while (j < 8)
  {
    draw(_x, _y + j - 7);
    delay(speed);
    j++;
    _panel->scan();
    _panel->fillRect(_x, _y, 8, 8, 0);
  }

  // movement for numb to appear we must redraw it since last command cleared quandrant (_panel->fillRect(_x, _y, 8, 8, 0))  draw(_x, _y);
  draw(_x, _y);
  _panel->scan();
}
//// useful only for NW a NE, not checked with others
void Number::scrollUp(byte _x, byte _y)
{
  int j = 0;
  while (j < 8)
  {
    draw(_x, _y - j + 7);
    delay(speed);
    j++;
    _panel->scan();
    _panel->fillRect(_x, _y, 8, 8, 0);
  }
  // movement for numb to appear we must redraw it since last command cleared quandrant (_panel->fillRect(_x, _y, 8, 8, 0))
  draw(_x, _y);
  _panel->scan();
}