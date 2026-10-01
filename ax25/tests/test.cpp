#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

#include <cstdint>
#include <string>
#include <vector>

#include "ax25/ax25.h"

namespace
{

AX25Config makeConfig()
{
    AX25Config config{};

    // Adapt these field names to your actual AX25Config definition.
    config.callSignFrom = "VK3ABC";
    config.ssidFrom = 1;
    config.callSignTo = "VK3JEZ";
    config.ssidTo = 0;

    return config;
}

AX25 makeAX25()
{
    return AX25{makeConfig()};
}

} // namespace

TEST_CASE("AX25 can encode and decode a text payload", "[ax25]")
{
    auto ax25 = makeAX25();

    const std::vector<uint8_t> payload{
        'H', 'e', 'l', 'l', 'o'
    };

    const auto frame = ax25.encode(payload);
    const auto decoded = ax25.decode(frame);

    REQUIRE(decoded.payload == payload);
}

TEST_CASE("AX25 preserves the payload exactly", "[ax25]")
{
    auto ax25 = makeAX25();

    const std::vector<uint8_t> payload{
        0x00,
        0x01,
        0x7E,
        0x7F,
        0x80,
        0xFE,
        0xFF
    };

    const auto frame = ax25.encode(payload);
    const auto decoded = ax25.decode(frame);

    REQUIRE(decoded.payload == payload);
}

TEST_CASE("AX25 supports an empty payload", "[ax25]")
{
    auto ax25 = makeAX25();

    const std::vector<uint8_t> payload;

    const auto frame = ax25.encode(payload);
    const auto decoded = ax25.decode(frame);

    REQUIRE(decoded.payload.empty());
}

TEST_CASE("AX25 preserves the configured source callsign", "[ax25]")
{
    const auto config = makeConfig();
    AX25 ax25{config};

    const std::vector<uint8_t> payload{'T', 'E', 'S', 'T'};

    const auto frame = ax25.encode(payload);
    const auto decoded = ax25.decode(frame);

    REQUIRE(decoded.fromCallSign == config.callSignFrom);
    REQUIRE(decoded.fromSSID == config.ssidFrom);
}

TEST_CASE("AX25 preserves the configured destination callsign", "[ax25]")
{
    const auto config = makeConfig();
    AX25 ax25{config};

    const std::vector<uint8_t> payload{'T', 'E', 'S', 'T'};

    const auto frame = ax25.encode(payload);
    const auto decoded = ax25.decode(frame);

    REQUIRE(decoded.toCallSign == config.callSignTo);
    REQUIRE(decoded.toSSID == config.ssidTo);
}

TEST_CASE("AX25 preserves both callsigns and SSIDs", "[ax25]")
{
    const auto config = makeConfig();
    AX25 ax25{config};

    const std::vector<uint8_t> payload{
        'A', 'X', '2', '5'
    };

    const auto frame = ax25.encode(payload);
    const auto decoded = ax25.decode(frame);

    REQUIRE(decoded.fromCallSign == config.callSignFrom);
    REQUIRE(decoded.fromSSID == config.ssidFrom);
    REQUIRE(decoded.toCallSign == config.callSignTo);
    REQUIRE(decoded.toSSID == config.ssidTo);
    REQUIRE(decoded.payload == payload);
}

TEST_CASE("AX25 encoded frame is not empty", "[ax25]")
{
    auto ax25 = makeAX25();

    const std::vector<uint8_t> payload{
        'T', 'E', 'S', 'T'
    };

    const auto frame = ax25.encode(payload);

    REQUIRE_FALSE(frame.empty());
}

TEST_CASE("AX25 round trip works for a larger payload", "[ax25]")
{
    auto ax25 = makeAX25();

    std::vector<uint8_t> payload;

    for (uint16_t i = 0; i < 256; ++i)
        payload.push_back(static_cast<uint8_t>(i));

    const auto frame = ax25.encode(payload);
    const auto decoded = ax25.decode(frame);

    REQUIRE(decoded.payload == payload);
}

std::vector<uint8_t> makePayload(std::size_t size) { std::vector<uint8_t> payload(size); for (std::size_t i = 0; i < size; ++i) payload[i] = static_cast<uint8_t>(i & 0xFF); return payload; }

TEST_CASE("AX25 round trip works for multiple payloads", "[ax25]")
{
    auto ax25 = makeAX25();

    const std::vector<std::vector<uint8_t>> payloads{
        {},
        {'A'},
        {'H', 'E', 'L', 'L', 'O'},
        {0x00, 0xFF, 0x7E, 0x01},
        {'V', 'K', '3', 'A', 'B', 'C'}
    };

    for (const auto& payload : payloads)
    {
        const auto frame = ax25.encode(payload);
        const auto decoded = ax25.decode(frame);

        REQUIRE(decoded.payload == payload);
    }

}
TEST_CASE("AX25 encode performance", "[benchmark][ax25]")
{
    AX25 ax25{makeConfig()};

    SECTION("16 bytes")
    {
        const auto payload = makePayload(16);

        BENCHMARK("encode 16 bytes")
        {
            return ax25.encode(payload);
        };
    }

    SECTION("64 bytes")
    {
        const auto payload = makePayload(64);

        BENCHMARK("encode 64 bytes")
        {
            return ax25.encode(payload);
        };
    }

    SECTION("256 bytes")
    {
        const auto payload = makePayload(256);

        BENCHMARK("encode 256 bytes")
        {
            return ax25.encode(payload);
        };
    }

    SECTION("1024 bytes")
    {
        const auto payload = makePayload(1024);

        BENCHMARK("encode 1024 bytes")
        {
            return ax25.encode(payload);
        };
    }

    SECTION("4096 bytes")
    {
        const auto payload = makePayload(4096);

        BENCHMARK("encode 4096 bytes")
        {
            return ax25.encode(payload);
        };
    }
}


TEST_CASE("AX25 decode performance", "[benchmark][ax25]")
{
    AX25 ax25{makeConfig()};

    SECTION("16 byte payload")
    {
        const auto payload = makePayload(16);
        const auto frame = ax25.encode(payload);

        BENCHMARK("decode 16 byte payload")
        {
            return ax25.decode(frame);
        };
    }

    SECTION("64 byte payload")
    {
        const auto payload = makePayload(64);
        const auto frame = ax25.encode(payload);

        BENCHMARK("decode 64 byte payload")
        {
            return ax25.decode(frame);
        };
    }

    SECTION("256 byte payload")
    {
        const auto payload = makePayload(256);
        const auto frame = ax25.encode(payload);

        BENCHMARK("decode 256 byte payload")
        {
            return ax25.decode(frame);
        };
    }

    SECTION("1024 byte payload")
    {
        const auto payload = makePayload(1024);
        const auto frame = ax25.encode(payload);

        BENCHMARK("decode 1024 byte payload")
        {
            return ax25.decode(frame);
        };
    }

    SECTION("4096 byte payload")
    {
        const auto payload = makePayload(4096);
        const auto frame = ax25.encode(payload);

        BENCHMARK("decode 4096 byte payload")
        {
            return ax25.decode(frame);
        };
    }
}


TEST_CASE("AX25 encode/decode round trip performance", "[benchmark][ax25]")
{
    AX25 ax25{makeConfig()};

    const auto payload = makePayload(256);

    BENCHMARK("encode + decode 256 bytes")
    {
        const auto frame = ax25.encode(payload);
        return ax25.decode(frame);
    };
}

