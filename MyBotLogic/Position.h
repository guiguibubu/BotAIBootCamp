#pragma once

struct Position {

   int x;
   int y;
   unsigned int tileID;

   Position() = default;
   Position(unsigned int tileID, int x, int y) : tileID{ tileID }, x { x }, y{ y } {};

   void move(int dx, int dy);
};

bool operator==(Position& position1, Position& position2);

bool operator!=(Position& position1, Position& position2);