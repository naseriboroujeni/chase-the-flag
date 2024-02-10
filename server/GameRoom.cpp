#include "GameUser.hpp"
#include "GameRoom.hpp"

#include <algorithm>

GameRoom::GameRoom(string name) {
   this->name = name;
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

vector<GameUser*> GameRoom::getAllUsers() {
   return this->joinedUsers;
}

GameRoom::~GameRoom() {
}
