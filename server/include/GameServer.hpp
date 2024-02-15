#ifndef GAME_SERVER_HPP
#define GAME_SERVER_HPP

#include <iostream>
#include <set>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "TagPool.hpp"
#include "GameRoom.hpp"
#include "GameUser.hpp"

using namespace std;
using namespace websocketpp;
using WsServer = server<config::asio>;
using ConnectionHdl = connection_hdl;

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
   void broadcastMessage(GameRoom* room, const string &message);

private:
   WsServer wsServer;

   TagPool* tagPool;

   GameRoom* lobby;
   map<string, GameRoom*> rooms;

   map<array<byte, 2>, GameUser*> users;

   /**
    * @brief Callback method called when a new client connection is opened.
    *
    * @param hdl The connection handle.
   */
   void onOpen(ConnectionHdl hdl);

   /**
    * @brief Sends the assigned tag to the connected user as a system message.
    * 
    * @param user Pointer to the GameUser object representing the connected user.
   */
   void sendTagMessage(GameUser* user);

   /**
    * @brief Callback method called when a message is received from a client.
    *
    * @param wsServer Pointer to the WebSocket server instance.
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void onMessage(WsServer *wsServer, ConnectionHdl hdl, WsServer::message_ptr msg);

   /**
    * @brief Callback method called when a client connection is closed.
    *
    * @param hdl The connection handle.
   */
   void onClose(ConnectionHdl hdl);

   /**
    * @brief This method gets called when there is any exception in handling client's messages.
    * Sends a brief of the exception occured in the server to the client.
    * 
    * @param hdl Connection to the client the exception occured for.
    * @param ex The exception that has been occured.
   */
   void onException(ConnectionHdl hdl, const std::exception& ex);

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
    * @param player Pointer to the GameUser object representing the player.
    * @param msg The received message.
   */
   void handleJoinRoom(GameUser* player, WsServer::message_ptr msg);

   /**
    * @brief Handles the LeaveRoom player update message.
    * 
    * @param player Pointer to the GameUser object representing the player.
    * @param msg The received message.
   */
   void handleLeaveRoom(GameUser* player, WsServer::message_ptr msg);

   /**
    * @brief Handles the Move player update message.
    * 
    * @param player Pointer to the GameUser object representing the player.
    * @param msg The received message.
   */
   void handleMove(GameUser* player, WsServer::message_ptr msg);

   /**
    * @brief Creates a location update message to send to player(s).
    * 
    * @return The location update message.
   */
   string createLocationMessage(GameUser* player);

   /**
    * @brief Handles the SendMessage player update message.
    * 
    * @param msgSender Pointer to the GameUser object representing the sender of the message.
    * @param msg The received message.
   */
   void handleChatMessage(GameUser* msgSender, WsServer::message_ptr msg);

   /**
    * @brief Handles setting SetUsername player update message.
    * 
    * @param player Pointer to the GameUser object representing the player.
    * @param msg The received message.
   */
   void handleSetUsername(GameUser* player, WsServer::message_ptr msg);
};

#endif // GAME_SERVER_HPP
