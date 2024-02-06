#ifndef GAME_SERVER_HPP
#define GAME_SERVER_HPP

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <set>

#include "GameRoom.hpp"
#include "GameUser.hpp"

using namespace std;
using namespace websocketpp;
using WsServer = server<config::asio>;
using ConnectionHdl = connection_hdl;


class GameServer {

public:
   /**
    * Constructor for the GameServer class.
   */
   GameServer();

   /**
    * Runs the WebSocket server on the specified port.
    * 
    * @param port The port on which the server should listen.
   */
   void run(uint16_t port);

   /**
    * Broadcasts a message to all connected clients.
    *
    * @param message The message to broadcast.
   */
   void broadcast_message(const string &message);

private:
   WsServer wsServer;
   set<ConnectionHdl, owner_less<ConnectionHdl>> connections;

   vector<GameRoom*> rooms;
   vector<GameUser*> users;

   /**
    * Callback method called when a message is received from a client.
    *
    * @param s Pointer to the WebSocket server instance.
    * @param hdl The connection handle.
    * @param msg The received message.
   */
   void on_message(WsServer *s, ConnectionHdl hdl, WsServer::message_ptr msg);

   /**
    * Callback method called when a new client connection is opened.
    *
    * @param hdl The connection handle.
   */
   void on_open(ConnectionHdl hdl);

   /**
    * Callback method called when a client connection is closed.
    *
    * @param hdl The connection handle.
   */
   void on_close(ConnectionHdl hdl);
};

#endif // GAME_SERVER_HPP
