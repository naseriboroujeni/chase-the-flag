#include "GameUser.hpp"
#include "GameRoom.hpp"

#include <algorithm>

GameRoom::GameRoom() {
}

void GameRoom::addUser(GameUser* joinedUser) {
   this->joinedUsers.push_back(joinedUser);
}

void GameRoom::removeUser(GameUser* gameUser) {
   auto it = remove(joinedUsers.begin(), joinedUsers.end(), gameUser);
   if (it != joinedUsers.end()) {
       joinedUsers.erase(it, joinedUsers.end());
   }
}

GameRoom::~GameRoom() {
}
