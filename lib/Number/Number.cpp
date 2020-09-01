#include "Number.h"
//https://stackoverflow.com/questions/37074763/arduino-class-array-member/50924520
Number::Number( ) {
}

Number::Number(Coord *coords, int nb_lines ) {
  _coordo = new Coord [nb_lines];
  for (int i = 0; i < nb_lines ; i++) {
    _coordo[i] = coords[i];
  }
  _nbreLigne = nb_lines;
}
