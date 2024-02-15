#include "GameClient.hpp"

using namespace std;

void handleUserCommands(GameClient* gameClient);

string SERVER_URI = "ws://localhost:9002";

int main() {

   GameClient* gameClient = new GameClient();

   thread t1(&GameClient::run, gameClient, SERVER_URI);

   handleUserCommands(gameClient);

   return 0;
}

void handleUserCommands(GameClient* gameClient) {

   bool done = false;
   while (!done) {
      string input;
      cout << "Enter a command (type '\\h' for available commands):\n";
      getline(cin, input);

      if (input == "\\c") {
         done = true;
         gameClient->closeConnection();
      }
      else if (input == "\\h") {
         cout << "Available commands:\n"
                 "\\h (for help)\n"
                 "\\n (for create room)\n"
                 "\\r (for get list of rooms)\n"
                 "\\m (for move)\n"
                 "\\s (to send a chat message)\n"
                 "\\j (for joinRoom)\n"
                 "\\l (for leaveRoom)\n"
                 "\\u (for set username)\n"
                 "\\c (for close)\n";
      }
      else if (input.size() == 4 && input.substr(0, 2) == "\\m") {
         // Process move command
         MoveType move = static_cast<MoveType>(input.at(3)); // Extract the direction after "\m "
         gameClient->sendMove(move);
      }
      else if (input.size() >= 4 && input.substr(0, 2) == "\\s") {
         // Process send message command
         string message = input.substr(3); // Extract the message after "\s "
         gameClient->sendChatMessage(message);
      }
      else if (input.size() >= 4 && input.substr(0, 2) == "\\j") {
         // Process joinRoom command
         string roomName = input.substr(3); // Extract the room's name after "\j "
         gameClient->joinRoom(roomName);
      }
      else if (input == "\\l") {
         // Process leave room command
         gameClient->leaveRoom();
      }
      else if (input.size() >= 4 && input.substr(0, 2) == "\\u") {
         // Process set username command
         string username = input.substr(3); // Extract the username after "\u "
         gameClient->setUsername(username);
      }
      else if (input.size() >= 4 && input.substr(0, 2) == "\\n") {
         // Process joinRoom command
         string roomName = input.substr(3); // Extract the room's name after "\j "
         gameClient->createRoom(roomName);
      }
      else if (input == "\\r") {
         // Process get list of rooms command
         gameClient->getListofRooms();
      }
      else {
         cout << "Unknown command. Type '\\h' for available commands.\n";
      }
   }
}
