#pragma once
#include <string>
#include <vector>
#include <cstdint>

#include "ax25Config.h"

class AX25 {
public:
    AX25(const AX25Config& config) : m_config(config) {}
    ~AX25() = default;

    std::vector<uint8_t> encode(const std::vector<uint8_t>& payload);
    
private:
    AX25Config m_config;

    std::vector<uint8_t> m_buildCallSign(const std::string& callsign, int ssid, bool last) const;
};