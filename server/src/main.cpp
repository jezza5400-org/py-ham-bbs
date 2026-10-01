#include "ax25/ax25Config.h"
#include "kiss/kissConfig.h"
#include <cstdint>
#include <spdlog/spdlog.h>
#include <vector>

#include <ax25/ax25.h>
#include <kiss/kiss.h>

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

    AX25Config config { 
        "VK3ABC",  // callSignFrom
        "VK3JEZ",  // callSignTo
        1,         // ssidFrom
        0          // ssidTo
    };

    KissConfig kissConfig {
        .type = KissConfigType::TCP,
        .config = KissConfigTCP {"localhost", 8001}
    };

    AX25* ax25 = new AX25(config);
    KissClient* kissClient = new KissClient(kissConfig);
    kissClient->connect();

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

    // Cleanup
    delete ax25;

    kissClient->disconnect();
    delete kissClient;

    return 0;
}   