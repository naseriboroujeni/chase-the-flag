#include "GameRoom.hpp"
#include "GameUser.hpp"

GameUser::GameUser(array<byte, 2> userTag, ConnectionHdl connection) {
   this->userTag = userTag;
   this->userName = "";
   this->connection = connection;
   this->location = new Location();
   this->collectedPoints = 0;
   this->joinedRoom = NULL;
}

array<byte, 2> GameUser::getTag() {
   return this->userTag;
}

ConnectionHdl GameUser::getConnection() {
   return this->connection;
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
