#include "GameClient.hpp"

#include "CommonEnums.hpp"

GameClient::GameClient() {

   wsClient.set_message_handler([this](ConnectionHdl hdl, WsClient::message_ptr msg)
                                { onMessage(hdl, msg); });
}

void GameClient::run(const string &uri) {

   connect(uri);
   wsClient.run();
}

void GameClient::sendMessage(const string message) {

   wsClient.send(con, message, frame::opcode::text);
}

void GameClient::closeConnection() {

   wsClient.close(con, websocketpp::close::status::normal, "done");
}

void GameClient::onMessage(ConnectionHdl hdl, WsClient::message_ptr msg) {

   if (msg->get_payload().empty()) {
      cerr << "Received an empty message from the server." << endl;
      return;
   }

   uint8_t messageTypeByte = msg->get_payload()[0];
   MessageType messageType = static_cast<MessageType>(messageTypeByte);

   switch (messageType) {
      case MessageType::SystemMessage:
         handleSystemMessage(hdl, msg);
         break;
      case MessageType::PlayerUpdate:
         // handlePlayerUpdateMessage(hdl, msg);
         break;
      default:
         cerr << "Received an invalid message type." << endl;
         break;
   }
}

void GameClient::handleSystemMessage(ConnectionHdl hdl, WsClient::message_ptr msg) {
   uint8_t systemMessageTypeByte = msg->get_payload()[1];
   SystemMessageType systemMessageType = static_cast<SystemMessageType>(systemMessageTypeByte);

   switch (systemMessageType) {
      case SystemMessageType::AssignTag:
         handleTagAssignement(msg);
         break;
      default:
         cerr << "Received an invalid system message type." << endl;
         break;
   }
}

void GameClient::connect(const string uri) {

   wsClient.init_asio();
   lib::error_code ec;
   con = wsClient.get_connection(uri, ec);
   wsClient.connect(con);
}

void GameClient::handleTagAssignement(WsClient::message_ptr msg) {

   this->tag = {static_cast<byte>(msg->get_payload()[2]), static_cast<byte>(msg->get_payload()[3])};
}

void GameClient::updatePlayerMovement(string move) {
   // TODO
}

void GameClient::joinRoom(string roomName) {
   // TODO
}

void GameClient::leaveRoom() {
   // TODO
}

void GameClient::setUsername(string username) {
   // TODO
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
