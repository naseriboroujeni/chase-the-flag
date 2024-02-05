#include "GameServer.hpp"


GameServer::GameServer() {

   wsServer.set_message_handler([this](ConnectionHdl hdl, WsServer::message_ptr msg)
                                { on_message(&wsServer, hdl, msg); });

   wsServer.set_open_handler([this](ConnectionHdl hdl)
                             { on_open(hdl); });

   wsServer.set_close_handler([this](ConnectionHdl hdl)
                              { on_close(hdl); });
}

void GameServer::run(uint16_t port) {

   wsServer.init_asio();
   wsServer.listen(port);
   wsServer.start_accept();
   wsServer.run();
}

void GameServer::broadcast_message(const string &message) {

   for (auto &connection : connections) {
      try {
         wsServer.send(connection, message, frame::opcode::text);
      }
      catch (const error_code &e) {
         cerr << "Error sending message to client: " << e.message() << endl;
      }
   }
}

void GameServer::on_message(WsServer *s, ConnectionHdl hdl, WsServer::message_ptr msg) {

   cout << "Received message from client: " << msg->get_payload() << endl;
   broadcast_message(msg->get_payload());
}

void GameServer::on_open(ConnectionHdl hdl) {

   connections.insert(hdl);
}

void GameServer::on_close(ConnectionHdl hdl) {

   connections.erase(hdl);
}
