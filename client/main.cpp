#include "GameClient.hpp"


int main() {

   GameClient gameClient;

   // Get URI from user input
   string uri;
   cout << "Enter WebSocket server URI (e.g., ws://localhost:9002): ";
   getline(cin, uri);

   // Run the client with the provided URI
   thread t1(&GameClient::run, &gameClient, uri);

   bool done = false;
   while (!done) {
      string input;
      getline(cin, input);
      if (input == "close") {
         done = true;
         gameClient.closeConnection();
      }
      else {
         gameClient.sendMessage(input);
      }
   }

   return 0;
}
