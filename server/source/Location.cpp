#include "Location.hpp"

#include "math.h"

Location::Location() {

   maxX = 10;
   maxY = 10;
   this->x = rand() % maxX;
   this->y = rand() % maxY;
}

void Location::goUp() {

   if (++this->y == this->maxY) {
      this->y = 0;
   }
}

void Location::goDown() {

   if (--this->y == -1) {
      this->y = this->maxY - 1;
   }
}

void Location::goRight() {

   if (++this->x == this->maxX) {
      this->x = 0;
   }
}

void Location::goLeft() {

   if (--this->x == -1) {
      this->x = this->maxX - 1;
   }
}

int Location::getY() {

   return this->y;
}

int Location::getX() {

   return this->x;
}

bool Location::operator==(const Location& other) const {

   return (x == other.x) && (y == other.y);
}
