#include "GameServer.hpp"

#include "GameExceptions.hpp"
#include "CommonEnums.hpp"

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
      this->sendMessage(user->getConnection(), message);
   }
}

void GameServer::onOpen(ConnectionHdl hdl) {

   array<byte, 2> tag = tagPool->allocateTag();
   GameUser* user = new GameUser(tag, hdl);
   user->setRoom(this->lobby);
   this->users[tag] = user;

   sendTagMessage(user);
}

void GameServer::sendTagMessage(GameUser* user) {

   // Construct the system message for assigning the tag
   string tagString = {static_cast<char>(user->getTag()[0]), static_cast<char>(user->getTag()[1])};
   string assignTagMessage = string(1, static_cast<char>(MessageType::SystemMessage)) +
                             string(1, static_cast<char>(SystemMessageType::AssignTag)) +
                             tagString;

   sendMessage(user->getConnection(), assignTagMessage);
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

   for(auto const& user: this->users) {
      GameUser* leavingPlayer = user.second;
      if (
         !leavingPlayer->getConnection().owner_before(hdl) &&
         !hdl.owner_before(leavingPlayer->getConnection()))
      {
         leavingPlayer->getRoom()->removeUser(leavingPlayer);
         users.erase(leavingPlayer->getTag());
         tagPool->deallocateTag(leavingPlayer->getTag());
         delete leavingPlayer;
         break;
      }
   }
}

void GameServer::onMessage(WsServer *wsServer, ConnectionHdl hdl, WsServer::message_ptr msg) {

   try {
      if (msg->get_payload().empty()) {
         throw InvalidMessageException();
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
            throw InvalidMessageException();
      }
   } catch(const std::exception& ex) {
      onException(hdl, ex);
   }
}

void GameServer::onException(ConnectionHdl hdl, const std::exception& ex) {

   string exceptionMessage = string(1, static_cast<char>(MessageType::SystemMessage)) +
                             string(1, static_cast<char>(SystemMessageType::Exception)) +
                             ex.what();
   
   sendMessage(hdl, exceptionMessage);
}

void GameServer::handleSystemMessage(ConnectionHdl hdl, WsServer::message_ptr msg) {

   uint8_t systemMessageTypeByte = msg->get_payload()[1];
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

   array<byte, 2> playerTag = {byte(msg->get_payload()[2]), byte(msg->get_payload()[3])};
   GameUser* player = users[playerTag];

   uint8_t playerUpdateTypeByte = msg->get_payload()[1];
   PlayerUpdateType playerUpdateType = static_cast<PlayerUpdateType>(playerUpdateTypeByte);

   switch (playerUpdateType) {
      case PlayerUpdateType::Move:
         handleMove(player, msg);
         break;
      case PlayerUpdateType::SendMessage:
         handleChatMessage(player, msg);
         break;
      case PlayerUpdateType::JoinRoom:
         handleJoinRoom(player, msg);
         break;
      case PlayerUpdateType::LeaveRoom:
         handleLeaveRoom(player, msg);
         break;
      case PlayerUpdateType::SetUsername:
         handleSetUsername(player, msg);
         break;
      default:
         throw InvalidMessageException();
    }
}

void GameServer::handleCreateRoom(ConnectionHdl hdl, WsServer::message_ptr msg) {

   string roomName = msg->get_payload().substr(2);
   auto it = rooms.find(roomName);
   if (it != rooms.end()) {
      throw DuplicatedRoomNameException();
   }

   GameRoom* newRoom = new GameRoom(roomName);
   rooms[roomName] = newRoom;
}

void GameServer::handleListRooms(ConnectionHdl hdl, WsServer::message_ptr msg) {

   if (msg->get_payload().size() != 2) {
      throw InvalidMessageException();
   }

   string roomNames = "";
   for(auto const& imap: this->rooms) {
      roomNames += imap.first + ", ";
   }

   string roomNamesMessage = string(1, static_cast<char>(MessageType::SystemMessage)) +
                             string(1, static_cast<char>(SystemMessageType::ListRooms)) +
                             roomNames.substr(0, roomNames.length() - 2);
   sendMessage(hdl, roomNamesMessage);
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

   if (!player->getRoom()->getIsPlaying()) {
      throw GameNotInProgressException();
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

   string locationMessage = createLocationMessage(player);

   if (player->getRoom()->getTargetPlayer() == player) {
      broadcastMessage(player->getRoom(), locationMessage);
   } else {
      checkWinner(player);
      sendMessage(player->getConnection(), locationMessage);
   }
}

void GameServer::checkWinner(GameUser* player) {

   if (player->getLocation()->operator==(*player->getRoom()->getTargetPlayer()->getLocation())) {
      cout << "Player " << player->getUsername() << " has won in room " << player->getRoom()->getName() << endl;
      player->addPoint();
      player->getRoom()->restartGame();
   }
}

string GameServer::createLocationMessage(GameUser* player) {

   string tagString = {static_cast<char>(player->getTag()[0]), static_cast<char>(player->getTag()[1])};
   string locationMessage = string(1, static_cast<char>(MessageType::PlayerUpdate)) +
                           string(1, static_cast<char>(PlayerUpdateType::Move)) +
                           tagString +
                           static_cast<char>(player->getLocation()->getX()) +
                           static_cast<char>(player->getLocation()->getY());

   return locationMessage;
}

void GameServer::handleChatMessage(GameUser* msgSender, WsServer::message_ptr msg) {

   string messageContent = msg->get_payload().substr(4);

   if (msgSender->getRoom() == nullptr) {
      throw NotInAValidRoomException();
   }

   // Construct the chat message to be broadcasted
   string tagString = {static_cast<char>(msgSender->getTag()[0]), static_cast<char>(msgSender->getTag()[1])};
   string formattedMessage = "[" + msgSender->getUsername() + "]: " + messageContent;
   string chatMessage = string(1, static_cast<char>(MessageType::PlayerUpdate)) +
                        string(1, static_cast<char>(PlayerUpdateType::SendMessage)) +
                        tagString +
                        formattedMessage;

   // Broadcast the message to all users in the same room
   broadcastMessage(msgSender->getRoom(), chatMessage);
}

void GameServer::handleSetUsername(GameUser* player, WsServer::message_ptr msg) {

   string username = msg->get_payload().substr(4);

   if (player->getRoom() == nullptr || player->getRoom() != lobby) {
      throw NotInAValidRoomException();
   }

   player->setUsername(username);
}
