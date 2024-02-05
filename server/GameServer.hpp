#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <set>

using namespace std;
using namespace websocketpp;
using WsServer = server<config::asio>;
using ConnectionHdl = connection_hdl;


class GameServer {

public:
   GameServer();

   void run(uint16_t port);
   void broadcast_message(const string &message);

private:
   WsServer wsServer;
   set<ConnectionHdl, owner_less<ConnectionHdl>> connections;

   void on_message(WsServer *s, ConnectionHdl hdl, WsServer::message_ptr msg);
   void on_open(ConnectionHdl hdl);
   void on_close(ConnectionHdl hdl);
};
