#include "GameServer.hpp"


const int SERVER_PORT = 9002;

int main() {

   GameServer gameServer;
   gameServer.run(SERVER_PORT);

   return 0;
}
