#ifndef GAME_SERVER_HPP
#define GAME_SERVER_HPP

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <set>

#include "TagPool.hpp"
#include "GameRoom.hpp"
#include "GameUser.hpp"

using namespace std;
using namespace websocketpp;
using WsServer = server<config::asio>;
using ConnectionHdl = connection_hdl;

/**
 * @brief Enumerates the different types of messages exchanged between the server and clients.
*/
enum class MessageType : uint8_t {
   SystemMessage = 0x00, /**< Represents a system message exchanged between the server and clients. */
   PlayerUpdate = 0x01   /**< Represents a player update message exchanged between the server and clients. */
};

/**
 * @brief Enumerates the different types of player update messages exchanged between the server and clients.
*/
enum class PlayerUpdateType : uint8_t {
   CreateRoom = 0x00, /**< Request for creating a new game room. */
   ListRooms = 0x01,  /**< Request for creating a new game room. */
   JoinRoom = 0x02,   /**< Request for joining a specific game room. */
   LeaveRoom = 0x03,  /**< Request for leaving the current game room. */
   Move = 0x04,       /**< Request for updating the player's movement in the game. */
   SendMessage = 0x05 /**< Request for sending a chat message. */
};

/**
 * @brief Represents the WebSocket server for the game.
*/
class GameServer {

public:
   /**
    * @brief Constructor for the GameServer class.
   */
   GameServer();

   /**
    * @brief Runs the WebSocket server on the specified port.
    * 
    * @param port The port on which the server should listen.
   */
   void run(uint16_t port);

   /**
    * @brief Broadcasts a message to all connected clients.
    *
    * @param message The message to broadcast.
   */
   void broadcast_message(const string &message);

private:
   WsServer wsServer;
   set<ConnectionHdl, owner_less<ConnectionHdl>> connections;

   TagPool* tagPool;
   vector<GameRoom*> rooms;
   map<array<byte, 2>, GameUser*> users;

   /**
    * @brief Callback method called when a new client connection is opened.
    *
    * @param hdl The connection handle.
   */
   void onOpen(ConnectionHdl hdl);

   /**
    * @brief Callback method called when a message is received from a client.
    *
    * @param s Pointer to the WebSocket server instance.
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void onMessage(WsServer *s, ConnectionHdl hdl, WsServer::message_ptr msg);

   /**
    * @brief Callback method called when a client connection is closed.
    *
    * @param hdl The connection handle.
   */
   void onClose(ConnectionHdl hdl);

   /**
    * @brief Sends a message to a specific connection.
    * 
    * @param connection The connection handle to which the message should be sent.
    * @param message The message to send.
   */
   void sendMessage(ConnectionHdl connection, string message);

   /**
    * @brief Handles incoming system messages.
    * 
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void handleSystemMessage(ConnectionHdl hdl, WsServer::message_ptr msg);

   /**
    * @brief Handles incoming player update messages.
    * 
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void handlePlayerUpdateMessage(ConnectionHdl hdl, WsServer::message_ptr msg);

   /**
    * @brief Handles the CreateRoom player update message.
    * 
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void handleCreateRoom(ConnectionHdl hdl, WsServer::message_ptr msg);

   /**
    * @brief Handles the ListRooms player update message.
    * 
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void handleListRooms(ConnectionHdl hdl, WsServer::message_ptr msg);

   /**
    * @brief Handles the JoinRoom player update message.
    * 
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void handleJoinRoom(ConnectionHdl hdl, WsServer::message_ptr msg);

   /**
    * @brief Handles the LeaveRoom player update message.
    * 
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void handleLeaveRoom(ConnectionHdl hdl, WsServer::message_ptr msg);

   /**
    * @brief Handles the Move player update message.
    * 
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void handleMove(ConnectionHdl hdl, WsServer::message_ptr msg);

   /**
    * @brief Handles the SendMessage player update message.
    * 
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void handleSendMessage(ConnectionHdl hdl, WsServer::message_ptr msg);
};

#endif // GAME_SERVER_HPP
