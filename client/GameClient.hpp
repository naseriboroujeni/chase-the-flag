#ifndef GAME_CLIENT_HPP
#define GAME_CLIENT_HPP

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <websocketpp/common/thread.hpp>

using namespace std;
using namespace websocketpp;
using WsClient = client<config::asio>;
using ConnectionHdl = connection_hdl;


class GameClient {

public:
   /**
    * Constructor for the GameClient class.
   */
   GameClient();

   /**
    * Runs the WebSocket client and connects to the specified URI.
    * 
    * @param uri The URI of the WebSocket server to connect to.
   */
   void run(const string &uri);

   /**
    * Sends a message to the connected WebSocket server.
    * 
    * @param message The message to send.
   */
   void sendMessage(const string message);

   /**
    * Closes the connection to the WebSocket server.
   */
   void closeConnection();

private:
   WsClient wsClient;
   WsClient::connection_ptr con;

   /**
    * Callback method called when a message is received from the server.
    * 
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void on_message(ConnectionHdl hdl, WsClient::message_ptr msg);

   /**
    * Connects to the specified WebSocket server URI.
    * 
    * @param uri The URI of the WebSocket server to connect to.
   */
   void connect(const string uri);
};

#endif // GAME_CLIENT_HPP
