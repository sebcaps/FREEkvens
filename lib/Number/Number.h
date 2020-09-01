
#ifndef __NUMBER_H
#define __NUMBER_H
#include "Arduino.h";


struct Coord {
  int x1;
  int y1;
  int x2;
  int y2;
};

class Number {
  public:
    Number();
    Number(Coord *coords, int nb_lines);
    int _nbreLigne;
    Coord *_coordo ;


};
#endif //__NUMBER_H
