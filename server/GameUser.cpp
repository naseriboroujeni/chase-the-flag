#include "GameRoom.hpp"
#include "GameUser.hpp"

GameUser::GameUser() {
   this->userName = "";
   this->location = new Location();
   this->collectedPoints = 0;
   this->joinedRoom = NULL;
}

void GameUser::setUserName(string userName) {
   this->userName = userName;
}

string GameUser::getUserName() {
   return this->userName;
}

Location* GameUser::getLocation() {
   return this->location;
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
