#include "GameRoom.hpp"
#include "GameUser.hpp"

#include <time.h>

GameUser::GameUser(string userName) {
   this->userName = userName;
   this-> x = rand() % 6;
   this-> y = rand() % 6;
   this->collectedPoints = 0;
   this->joinedRoom = NULL;
}

void GameUser::setRoom(GameRoom* joinedRoom) {
   this->joinedRoom = joinedRoom;
   this->joinedRoom->addUser(this);
}

GameRoom* GameUser::getRoom() {
   return this->joinedRoom;
}

GameUser::~GameUser() {
}
