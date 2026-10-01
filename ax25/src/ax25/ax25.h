#pragma once
#include <string>
#include <vector>
#include <cstdint>

#include "ax25Config.h"

struct DecodedAX25Frame {
    std::string fromCallSign;
    int fromSSID;
    std::string toCallSign;
    int toSSID;
    std::vector<uint8_t> payload;
};
class AX25 {
public:
    AX25(const AX25Config& config) : m_config(config) {}
    ~AX25() = default;

    std::vector<uint8_t> encode(const std::vector<uint8_t>& payload);
    DecodedAX25Frame decode(const std::vector<uint8_t>& frame);  
    
private:
    AX25Config m_config;

    std::vector<uint8_t> m_buildCallSign(const std::string& callsign, int ssid, bool last) const;
};