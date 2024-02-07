#include "GameServer.hpp"

GameServer::GameServer() {

   tagPool = new TagPool();

   wsServer.set_message_handler([this](ConnectionHdl hdl, WsServer::message_ptr msg)
                                { onMessage(&wsServer, hdl, msg); });

   wsServer.set_open_handler([this](ConnectionHdl hdl)
                             { onOpen(hdl); });

   wsServer.set_close_handler([this](ConnectionHdl hdl)
                              { onClose(hdl); });
}

void GameServer::run(uint16_t port) {

   wsServer.init_asio();
   wsServer.listen(port);
   wsServer.start_accept();
   wsServer.run();
}

void GameServer::broadcast_message(const string &message) {

   for (auto &connection : connections) {
      this->sendMessage(connection, message);
   }
}

void GameServer::onOpen(ConnectionHdl hdl) {

   array<byte, 2> tag = tagPool->allocateTag();
   GameUser *user = new GameUser();
   users[tag] = user;

   try {
      wsServer.send(hdl, tag.data(), 2, frame::opcode::binary);
   } catch (const error_code &e) {
      cerr << "Error sending tag to client: " << e.message() << endl;
   }

   connections.insert(hdl);
}

void GameServer::sendMessage(ConnectionHdl connection, string message) {
   try {
      wsServer.send(connection, message, frame::opcode::text);
   }
   catch (const error_code &e) {
      cerr << "Error sending message to client: " << e.message() << endl;
   }
}

void GameServer::onClose(ConnectionHdl hdl) {

   connections.erase(hdl);
}

void GameServer::onMessage(WsServer *s, ConnectionHdl hdl, WsServer::message_ptr msg) {

   uint8_t messageTypeByte = msg->get_payload()[0];

   if (messageTypeByte == 0x00) {
      handleSystemMessage(hdl, msg);
   } else if (messageTypeByte == 0x01) {
      handlePlayerUpdateMessage(hdl, msg);
   }
}

void GameServer::handleSystemMessage(ConnectionHdl hdl, WsServer::message_ptr msg) {
   // TODO
}

void GameServer::handlePlayerUpdateMessage(ConnectionHdl hdl, WsServer::message_ptr msg) {

   array<byte, 2> playerTag = {byte(msg->get_payload()[1]), byte(msg->get_payload()[2])};
   GameUser* player = users[playerTag];

   uint8_t playerUpdateTypeByte = msg->get_payload()[3];
   PlayerUpdateType playerUpdateType = static_cast<PlayerUpdateType>(playerUpdateTypeByte);

   switch (playerUpdateType) {
      case PlayerUpdateType::CreateRoom:
         handleCreateRoom(hdl, msg);
         break;
      case PlayerUpdateType::ListRooms:
         handleListRooms(hdl, msg);
         break;
      case PlayerUpdateType::JoinRoom:
         handleJoinRoom(hdl, msg);
         break;
      case PlayerUpdateType::LeaveRoom:
         handleLeaveRoom(hdl, msg);
         break;
      case PlayerUpdateType::Move:
         handleMove(hdl, msg);
         break;
      case PlayerUpdateType::SendMessage:
         handleSendMessage(hdl, msg);
         break;
      default:
         cerr << "Unknown player update type: " << static_cast<int>(playerUpdateType) << endl;
    }
}

void GameServer::handleCreateRoom(ConnectionHdl hdl, WsServer::message_ptr msg) {
   // TODO
}

void GameServer::handleListRooms(ConnectionHdl hdl, WsServer::message_ptr msg) {
   // TODO
}

void GameServer::handleJoinRoom(ConnectionHdl hdl, WsServer::message_ptr msg) {
   // TODO
}

void GameServer::handleLeaveRoom(ConnectionHdl hdl, WsServer::message_ptr msg) {
   // TODO
}

void GameServer::handleMove(ConnectionHdl hdl, WsServer::message_ptr msg) {
   // TODO
}

void GameServer::handleSendMessage(ConnectionHdl hdl, WsServer::message_ptr msg) {
   // TODO
}
