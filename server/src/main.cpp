#include "ax25/ax25Config.h"
#include <cstdint>
#include <spdlog/spdlog.h>
#include <vector>

#include <ax25/ax25.h>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/server.hpp>

using Client = websocketpp::server<websocketpp::config::asio_client>;

void printHex(const std::vector<uint8_t>& data) {
    std::string hex;
    hex.reserve(data.size() * 3);
    for (uint8_t byte : data) {
        hex += fmt::format("{:02X} ", byte);
    }
    spdlog::info("Data (hex): {}", hex);
}

int main(int argc, char** argv) {
    Client client;
    client.init_asio();

    AX25Config config;
    config.callSignFrom = "VK3ABC";
    config.ssidFrom = 1;
    config.callSignTo = "VK3JEZ";
    config.ssidTo = 0;

    AX25* ax25 = new AX25(config);

    client.set_open_handler([&client](websocketpp::connection_hdl hdl) {
        spdlog::info("WebSocket connection opened");
        // You can send messages here if needed
    });

    client.set_message_handler([&client, &ax25](websocketpp::connection_hdl hdl, Client::message_ptr msg) {
        auto packet = ax25->encode(std::vector<uint8_t>(msg->get_payload().begin(), msg->get_payload().end()));
        printHex(packet);
    });

    spdlog::info("Starting WebSocket server on port 8080 at ws://localhost:8080");
    client.listen(8080);
    client.start_accept();
    client.run();

    return 0;
}   