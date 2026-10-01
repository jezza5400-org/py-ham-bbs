#pragma once
#include <string>
#include <spdlog/spdlog.h>

struct AX25Config {
    std::string callSignFrom;
    std::string callSignTo;
    int ssidFrom;
    int ssidTo;

    void print() const {
        spdlog::info("AX.25 Config: From {}-{} To {}-{}", callSignFrom, ssidFrom, callSignTo, ssidTo);
    }
};