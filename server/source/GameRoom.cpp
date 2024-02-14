#include "GameUser.hpp"
#include "GameRoom.hpp"

#include <algorithm>

#include "math.h"

GameRoom::GameRoom(string name) {

   this->name = name;
   this->isPlaying = false;
   this->targetPlayer = NULL;
}

void GameRoom::addUser(GameUser* joinedUser) {

   this->joinedUsers.push_back(joinedUser);
   updatePlayingStatus();
}

void GameRoom::removeUser(GameUser* gameUser) {

   auto it = remove(joinedUsers.begin(), joinedUsers.end(), gameUser);
   if (it != joinedUsers.end()) {
       joinedUsers.erase(it, joinedUsers.end());
   }
   updatePlayingStatus();
}

void GameRoom::updatePlayingStatus() {

   if (!this->isPlaying && this->joinedUsers.size() >= 3) {
      isPlaying = true;
      int randomPlayerIndex = rand() % this->joinedUsers.size();
      this->targetPlayer = this->joinedUsers[randomPlayerIndex];
      return;
   }
   if (this->isPlaying && this->joinedUsers.size() < 3) {
      isPlaying = false;
      this->targetPlayer = NULL;
      return;
   }
}

vector<GameUser*> GameRoom::getAllUsers() {

   return this->joinedUsers;
}

GameRoom::~GameRoom() {
}
