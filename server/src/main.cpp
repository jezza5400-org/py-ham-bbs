#include "ax25/ax25Config.h"
#include <cstdio>
#include <spdlog/spdlog.h>


int main(int argc, char** argv) {
    AX25Config config {
        .callSignFrom = "N0CALL",
        .callSignTo = "NOCALL",
        .ssidFrom = 0,
        .ssidTo = 0
    };

    config.print();
    return 0;
}   