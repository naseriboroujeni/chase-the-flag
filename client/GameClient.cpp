#include "GameClient.hpp"


GameClient::GameClient() {

   wsClient.set_message_handler([this](ConnectionHdl hdl, WsClient::message_ptr msg)
                                { on_message(hdl, msg); });
}

void GameClient::run(const string &uri) {

   connect(uri);
   wsClient.run();
}

void GameClient::sendMessage(const string message) {

   wsClient.send(con, message, frame::opcode::text);
}

void GameClient::closeConnection() {

   wsClient.close(con, websocketpp::close::status::normal, "done");
}

void GameClient::on_message(ConnectionHdl hdl, WsClient::message_ptr msg) {

   cout << "Received message from server: " << msg->get_payload() << endl;
}

void GameClient::connect(const string uri) {

   wsClient.init_asio();
   lib::error_code ec;
   con = wsClient.get_connection(uri, ec);
   wsClient.connect(con);
}