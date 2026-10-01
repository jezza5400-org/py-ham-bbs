#include <cstdint>
#include <spdlog/spdlog.h>
#include <vector>

#include "ax25/ax25.h"

int main(int argc, char** argv) {
    AX25Config config {
        .callSignFrom = "N0CALL",
        .callSignTo = "NOCALL",
        .ssidFrom = 0,
        .ssidTo = 0
    };

    config.print();
    
    AX25 ax25(config);
    
    std::vector<uint8_t> payload = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello" in ASCII

    auto out = ax25.encode(payload);

    return 0;
}   