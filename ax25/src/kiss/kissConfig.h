#pragma once
#include <string>
#include <variant>

enum class KissConfigType {
    TCP,
    SERIAL
};

struct KissConfigTCP {
    std::string host;
    int port;
};

struct KissConfigSerial {
    std::string device;
    int baudRate;
};

using KissConfigVariant =
    std::variant<KissConfigTCP, KissConfigSerial>;

struct KissConfig {
    KissConfigType type;
    KissConfigVariant config;
};