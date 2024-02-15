#include "GameRoom.hpp"
#include "GameUser.hpp"

GameUser::GameUser(array<byte, 2> userTag, ConnectionHdl connection) {

   this->userTag = userTag;
   this->username = "";
   this->connection = connection;
   this->location = new Location();
   this->collectedPoints = 0;
   this->joinedRoom = NULL;
}

array<byte, 2> GameUser::getTag() {

   return this->userTag;
}

void GameUser::addPoint() {

   this->collectedPoints++;
}

ConnectionHdl GameUser::getConnection() {

   return this->connection;
}

void GameUser::setUsername(string username) {

   this->username = username;
}

string GameUser::getUsername() {

   return this->username;
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

   delete location;
}
