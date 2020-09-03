
#ifndef __NUMBER_H
#define __NUMBER_H
#include "Arduino.h"
#include "FrekvensPanel.h"
#define speed 100
struct Coord
{
  int x1;
  int y1;
  int x2;
  int y2;
};

class Number
{
public:
  Number();
  Number(Coord *coords, int nb_lines);
  void begin(FrekvensPanel *panel);
  void draw(int x, int y);
  int _nbreLigne;
  Coord *_coordo;
  void scrollLeft(byte quad_x, byte quad_y);
  void scrollRight(byte quad_x, byte quad_y);
  void scrollUp(byte quad_x, byte quad_y);
  void scrollDown(byte quad_x, byte quad_y);

private:
  FrekvensPanel *_panel;
};
#endif //__NUMBER_H
