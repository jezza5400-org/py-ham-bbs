#include <catch2/catch_test_macros.hpp>
#include <ax25/ax25.h>

AX25Config config {
    .callSignFrom = "N0CALL",
    .callSignTo = "NOCALL",
    .ssidFrom = 0,
    .ssidTo = 0
};

TEST_CASE("Basic ax25 test")
{
    AX25 ax25(config);
    
    std::vector<uint8_t> payload = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello" in ASCII
    auto out = ax25.encode(payload);

    REQUIRE(out.size() == 15 + payload.size()); // 15 bytes for header + payload size

    auto decoded = ax25.decode(out);
    REQUIRE(decoded.fromCallSign == "N0CALL");
    REQUIRE(decoded.toCallSign == "NOCALL");
    REQUIRE(decoded.fromSSID == 0);
    REQUIRE(decoded.toSSID == 0);
    REQUIRE(decoded.payload == payload);
}