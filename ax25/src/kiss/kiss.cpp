#include "kiss.h"
#include <spdlog/spdlog.h>

void KissClient::connect() {
    if (std::holds_alternative<KissConfigTCP>(m_config.config)) {
        auto tcpConfig = std::get<KissConfigTCP>(m_config.config);
        spdlog::info("Connecting to KISS TCP server at {}:{}", tcpConfig.host, tcpConfig.port);
        
    } else if (std::holds_alternative<KissConfigSerial>(m_config.config)) {
        auto serialConfig = std::get<KissConfigSerial>(m_config.config);
        spdlog::info("Connecting to KISS Serial device at {} with baud rate {}", serialConfig.device, serialConfig.baudRate);
        spdlog::warn("Serial connection is not yet implemented");
        // Implement Serial connection logic here
    } else {
        spdlog::error("Unknown KISS configuration type");
    }
}

void KissClient::disconnect() {
    spdlog::info("Disconnecting from KISS client");
    // Implement disconnection logic here
}