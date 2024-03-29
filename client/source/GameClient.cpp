#include "GameClient.hpp"

GameClient::GameClient() {

   this->wsClient = new WsClient();

   wsClient->set_message_handler([this](ConnectionHdl hdl, WsClient::message_ptr msg)
                                { onMessage(hdl, msg); });

   wsClient->clear_access_channels(
      websocketpp::log::alevel::frame_header |
      websocketpp::log::alevel::frame_payload
   );
}

void GameClient::run(const string &uri) {

   connect(uri);
   wsClient->run();
}

void GameClient::sendMessage(const string message) {

   wsClient->send(con, message, frame::opcode::text);
}

void GameClient::closeConnection() {

   wsClient->close(con, websocketpp::close::status::normal, "done");
}

void GameClient::onMessage(ConnectionHdl hdl, WsClient::message_ptr msg) {

   if (msg->get_payload().empty()) {
      cerr << "Received an empty message from the server." << endl;
      return;
   }

   string payload = msg->get_payload();
   uint8_t messageTypeByte = payload[0];
   MessageType messageType = static_cast<MessageType>(messageTypeByte);

   switch (messageType) {
      case MessageType::SystemMessage:
         handleSystemMessage(payload.substr(1));
         break;
      case MessageType::PlayerUpdate:
         handlePlayerUpdateMessage(payload.substr(1));
         break;
      default:
         cerr << "Received an invalid message type." << endl;
         break;
   }
}

void GameClient::handleSystemMessage(string msg) {

   uint8_t systemMessageTypeByte = msg[0];
   SystemMessageType systemMessageType = static_cast<SystemMessageType>(systemMessageTypeByte);

   switch (systemMessageType) {
      case SystemMessageType::AssignTag:
         handleTagAssignement(msg.substr(1));
         break;
      case SystemMessageType::ListRooms:
         handleListRooms(msg.substr(1));
         break;
      case SystemMessageType::Exception:
         handleExceptionMessage(msg.substr(1));
         break;
      default:
         cerr << "Received an invalid system message type." << endl;
         break;
   }
}

void GameClient::connect(const string uri) {

   wsClient->init_asio();
   lib::error_code ec;
   con = wsClient->get_connection(uri, ec);
   wsClient->connect(con);
}

void GameClient::handleTagAssignement(string tag) {

   this->tag = {static_cast<byte>(tag[0]), static_cast<byte>(tag[1])};
}

void GameClient::handlePlayerUpdateMessage(string msg) {

   uint8_t playerUpdateTypeByte = msg[0];
   PlayerUpdateType playerUpdateType = static_cast<PlayerUpdateType>(playerUpdateTypeByte);

   switch (playerUpdateType) {
      case PlayerUpdateType::SendMessage:
         handleRecievedChatMessage(msg.substr(3));
         break;
      case PlayerUpdateType::Move:
         handleRecievedLocationMessage(msg.substr(1, 2), msg.substr(3));
         break;
      default:
         cerr << "Received an invalid system message type." << endl;
   }
}

void GameClient::handleRecievedLocationMessage(string tag, string location) {

   string tagString = {static_cast<char>(this->tag[0]), static_cast<char>(this->tag[1])};

   if (tagString == tag) {
      cout << "Your current location is: ";
   } else {
      cout << "Target current location is: ";
   }
   cout << int(location[0]) << ", " << int(location[1]) << endl;
}

void GameClient::handleRecievedChatMessage(string chatMessage) {

   cout << chatMessage << endl;
}

void GameClient::handleListRooms(string roomNames) {

   cout << roomNames << endl;
}

void GameClient::sendMove(MoveType move) {

   // Construct the player update message for sending movement
   string tagString = {static_cast<char>(tag[0]), static_cast<char>(tag[1])};
   string movementMessage = string(1, static_cast<char>(MessageType::PlayerUpdate)) +
                           string(1, static_cast<char>(PlayerUpdateType::Move)) +
                           tagString +
                           static_cast<char>(move);

   sendMessage(movementMessage);
}

void GameClient::joinRoom(string roomName) {

   // Construct the player update message for joining the room
   string tagString = {static_cast<char>(tag[0]), static_cast<char>(tag[1])};
   string joinRoomMessage = string(1, static_cast<char>(MessageType::PlayerUpdate)) +
                           string(1, static_cast<char>(PlayerUpdateType::JoinRoom)) +
                           tagString +
                           roomName;

   sendMessage(joinRoomMessage);
}

void GameClient::leaveRoom() {

   // Construct the player update message for leaving the room
   string tagString = {static_cast<char>(tag[0]), static_cast<char>(tag[1])};
   string leaveRoomMessage = string(1, static_cast<char>(MessageType::PlayerUpdate)) +
                           string(1, static_cast<char>(PlayerUpdateType::LeaveRoom)) +
                           tagString;

   sendMessage(leaveRoomMessage);
}

void GameClient::setUsername(string username) {

   // Construct the player update message for setting username
   string tagString = {static_cast<char>(tag[0]), static_cast<char>(tag[1])};
   string setUsernameMessage = string(1, static_cast<char>(MessageType::PlayerUpdate)) +
                               string(1, static_cast<char>(PlayerUpdateType::SetUsername)) +
                               tagString +
                               username;

   sendMessage(setUsernameMessage);
}

void GameClient::getListofRooms() {

   // Construct the system message for listing rooms
   string listRoomsMessage = string(1, static_cast<char>(MessageType::SystemMessage)) +
                              string(1, static_cast<char>(SystemMessageType::ListRooms));

   sendMessage(listRoomsMessage);
}

void GameClient::sendChatMessage(string message) {

   // Construct the player update message for sending a chat message
   string tagString = {static_cast<char>(tag[0]), static_cast<char>(tag[1])};
   string chatMessage = string(1, static_cast<char>(MessageType::PlayerUpdate)) +
                        string(1, static_cast<char>(PlayerUpdateType::SendMessage)) +
                        tagString +
                        message;

   sendMessage(chatMessage);
}

void GameClient::createRoom(string roomName) {

   // Construct the system message for creating a room
   string createRoomMessage = string(1, static_cast<char>(MessageType::SystemMessage)) +
                              string(1, static_cast<char>(SystemMessageType::CreateRoom)) +
                              roomName;

   sendMessage(createRoomMessage);
}

void GameClient::handleExceptionMessage(string exceptionMessage) {

   cout << "Exception Occured: " << exceptionMessage << endl;
}
