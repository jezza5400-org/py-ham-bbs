#include <cstdint>
#include <spdlog/spdlog.h>
#include <vector>

#include <ax25/ax25.h>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

using Client = websocketpp::client<websocketpp::config::asio_client>;

int main(int argc, char** argv) {
    Client client;
    client.init_asio();

    client.set_open_handler([&client](websocketpp::connection_hdl hdl) {
        spdlog::info("WebSocket connection opened");
        // You can send messages here if needed
    });

    client.set_message_handler([&client](websocketpp::connection_hdl hdl, Client::message_ptr msg) {
        spdlog::info("Received message: {}", msg->get_payload());
        // Handle incoming messages here
    });

    websocketpp::lib::error_code ec;
    auto con = client.get_connection("ws://localhost:8080", ec);
    if (ec) {
        spdlog::error("Could not create connection because: {}", ec.message());
        return 1;
    };

    return 0;
}   