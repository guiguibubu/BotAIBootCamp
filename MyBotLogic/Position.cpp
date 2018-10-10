#include "Position.h"

bool operator==(Position& position1, Position& position2) {
   return position1.x == position2.x && position1.y == position2.y;
}

bool operator!=(Position& position1, Position& position2) {
   return !( position1 == position2 );
}

void Position::move(int dx, int dy){
	x += dx;
	y += dy;
}