#include "GameServer.hpp"

#include "GameExceptions.hpp"

GameServer::GameServer() {

   this->lobby = new GameRoom("Lobby");
   this->rooms["LOBBY"] = lobby;

   this->tagPool = new TagPool();

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

void GameServer::broadcastMessage(GameRoom* room, const string &message) {

   for (auto &user : room->getAllUsers()) {
      this->sendMessage(*user->getConnection(), message);
   }
}

void GameServer::onOpen(ConnectionHdl hdl) {

   array<byte, 2> tag = tagPool->allocateTag();
   GameUser *user = new GameUser(tag, &hdl);
   user->setRoom(this->lobby);
   this->users[tag] = user;

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

void GameServer::onMessage(WsServer *wsServer, ConnectionHdl hdl, WsServer::message_ptr msg) {

   if (msg->get_payload().empty()) {
      cerr << "Received empty message from client." << endl;
      return;
   }

   uint8_t messageTypeByte = msg->get_payload()[0];
   MessageType messageType = static_cast<MessageType>(messageTypeByte);

   switch (messageType) {
      case MessageType::SystemMessage:
         handleSystemMessage(hdl, msg);
         break;
      case MessageType::PlayerUpdate:
         handlePlayerUpdateMessage(hdl, msg);
         break;
      default:
         cerr << "Unknown message type: " << static_cast<int>(messageType) << endl;
         break;
   }
}

void GameServer::handleSystemMessage(ConnectionHdl hdl, WsServer::message_ptr msg) {

   uint8_t systemMessageTypeByte = msg->get_payload()[3];
   SystemMessageType systemMessageType = static_cast<SystemMessageType>(systemMessageTypeByte);

   switch (systemMessageType) {
      case SystemMessageType::CreateRoom:
         handleCreateRoom(hdl, msg);
         break;
      case SystemMessageType::ListRooms:
         handleListRooms(hdl, msg);
         break;
      default:
         throw InvalidMessageException();
   }
}

void GameServer::handlePlayerUpdateMessage(ConnectionHdl hdl, WsServer::message_ptr msg) {

   array<byte, 2> playerTag = {byte(msg->get_payload()[1]), byte(msg->get_payload()[2])};
   GameUser* player = users[playerTag];

   uint8_t playerUpdateTypeByte = msg->get_payload()[3];
   PlayerUpdateType playerUpdateType = static_cast<PlayerUpdateType>(playerUpdateTypeByte);

   switch (playerUpdateType) {
      case PlayerUpdateType::Move:
         handleMove(player, msg);
         break;
      case PlayerUpdateType::SendMessage:
         handleSendMessage(player, msg);
         break;
      case PlayerUpdateType::JoinRoom:
         handleJoinRoom(player, msg);
         break;
      case PlayerUpdateType::LeaveRoom:
         handleLeaveRoom(player, msg);
         break;
      default:
         throw InvalidMessageException();
    }
}

void GameServer::handleCreateRoom(ConnectionHdl hdl, WsServer::message_ptr msg) {
   string roomName = msg->get_payload().substr(4);
   auto it = rooms.find(roomName);
   if (it != rooms.end()) {
      throw DuplicatedRoomNameException();
   }

   GameRoom* newRoom = new GameRoom(roomName);
   rooms[roomName] = newRoom;
}

void GameServer::handleListRooms(ConnectionHdl hdl, WsServer::message_ptr msg) {
   if (msg->get_payload().size() != 4) {
      throw InvalidMessageException();
   }
   // TODO
}

void GameServer::handleJoinRoom(GameUser* player, WsServer::message_ptr msg) {
   if (player->getRoom() == nullptr || player->getRoom() != lobby) {
      throw NotInAValidRoomException();
   }

   string roomName = msg->get_payload().substr(4);
   auto it = rooms.find(roomName);
   if (it == rooms.end()) {
      throw RoomNotFoundException();
   }

   GameRoom* joiningRoom = it->second;
   lobby->removeUser(player);
   player->setRoom(joiningRoom);
}

void GameServer::handleLeaveRoom(GameUser* player, WsServer::message_ptr msg) {

   if (msg->get_payload().size() != 4) {
      throw InvalidMessageException();
   }

   if (player->getRoom() == nullptr || player->getRoom() == lobby) {
      throw NotInAValidRoomException();
   }

   player->getRoom()->removeUser(player);
   player->setRoom(lobby);
}

void GameServer::handleMove(GameUser* player, WsServer::message_ptr msg) {

   if (player->getRoom() == nullptr || player->getRoom() == lobby) {
      throw NotInAValidRoomException();
   }

   if (msg->get_payload().size() != 5) {
      throw InvalidMessageException();
   }

   // Extract the move type from the message payload
   MoveType moveType = static_cast<MoveType>(msg->get_payload()[4]);

   // Update the player's location based on the move type
   switch (moveType) {
      case MoveType::UP:
         player->getLocation()->goUp();
         break;
      case MoveType::DOWN:
         player->getLocation()->goDown();
         break;
      case MoveType::RIGHT:
         player->getLocation()->goRight();
         break;
      case MoveType::LEFT:
         player->getLocation()->goLeft();
         break;
      default:
         throw InvalidMessageException();
   }
}

void GameServer::handleSendMessage(GameUser* msgSender, WsServer::message_ptr msg) {
   string messageContent = msg->get_payload().substr(4);

   if (msgSender->getRoom() == nullptr) {
      throw NotInAValidRoomException();
   }

   // Construct the formatted message to be broadcasted
   string formattedMessage = "[" + msgSender->getUserName() + "]: " + messageContent;

   // Broadcast the message to all users in the same room
   broadcastMessage(msgSender->getRoom(), formattedMessage);
}
