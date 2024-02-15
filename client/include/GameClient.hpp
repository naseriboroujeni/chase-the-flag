#ifndef GAME_CLIENT_HPP
#define GAME_CLIENT_HPP

#include <iostream>
#include <string>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>

#include "CommonEnums.hpp"

using namespace std;
using namespace websocketpp;
using WsClient = client<config::asio>;
using ConnectionHdl = connection_hdl;

/**
 * @brief Represents a WebSocket client for a multiplayer game.
 * 
 * The GameClient class manages the WebSocket connection to a game server,
 * allowing players to interact with the game, send messages, and receive updates.
 * It provides methods for various actions such as sending messages, joining game rooms,
 * updating player movement, and handling server responses.
*/
class GameClient {

public:
   /**
    * @brief Constructor for the GameClient class.
   */
   GameClient();

   /**
    * @brief Runs the WebSocket client and connects to the specified URI.
    * 
    * @param uri The URI of the WebSocket server to connect to.
   */
   void run(const string &uri);

   /**
    * @brief Sends a message to the connected WebSocket server.
    * 
    * @param message The message to send.
   */
   void sendMessage(const string message);

   /**
    * @brief Closes the connection to the WebSocket server.
   */
   void closeConnection();

   /**
    * @brief Updates the player's movement in the game.
    * 
    * @param move The movement command (e.g., "u" for up, "d" for down).
   */
   void sendMove(MoveType move);

   /**
    * @brief Joins a specific game room.
    * 
    * @param roomName The name of the room to join.
   */
   void joinRoom(string roomName);

   /**
    * @brief Leaves the current game room.
   */
   void leaveRoom();

   /**
    * @brief Sets the username for the player.
    * 
    * @param username The desired username.
   */
   void setUsername(string username);

   /**
    * @brief Retrieves the list of available game rooms.
   */
   void getListofRooms();

   /**
    * @brief Sends a chat message to the connected WebSocket server.
    * 
    * @param message The content of the chat message to send.
   */
   void sendChatMessage(string message);

   /**
    * @brief Creates a new game room.
    * 
    * @param roomName The name of the new room to create.
   */
   void createRoom(string roomName);


private:
   WsClient* wsClient;
   WsClient::connection_ptr con;

   array<byte, 2> tag;

   /**
    * @brief Callback method called when a message is received from the server.
    * 
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void onMessage(ConnectionHdl hdl, WsClient::message_ptr msg);

   /**
    * Connects to the specified WebSocket server URI.
    * 
    * @param uri The URI of the WebSocket server to connect to.
   */
   void connect(const string uri);

   /**
    * @brief Handles system messages received from the server.
    * 
    * @param msg The received system message.
   */
   void handleSystemMessage(string msg);

   /**
    * @brief Handles tag assignment messages received from the server.
    * 
    * @param tag The received tag assignment message.
   */
   void handleTagAssignement(string tag);

   /**
    * @brief Handles player update messages received from the server.
    * 
    * @param msg The received player update message.
   */
   void handlePlayerUpdateMessage(string msg);

   /**
    * @brief Handles location messages received from the server.
    * 
    * @param tag The tag associated with the player the location is related to.
    * @param location The location information received from the server.
   */
   void handleRecievedLocationMessage(string tag, string location);

   /**
    * @brief Handles chat messages received from the server.
    * 
    * @param chatMessage The received chat message.
   */
   void handleRecievedChatMessage(string chatMessage);

   /**
    * @brief Handles room names received from the server.
    * 
    * @param chatMessage The received room names separated by ','.
   */
   void handleListRooms(string roomNames);

   /**
    * @brief Handles exception message recieved from the server.
    * 
    * @param exceptionMessage The recieved exception message.
   */
   void handleExceptionMessage(string exceptionMessage);
};

#endif // GAME_CLIENT_HPP
