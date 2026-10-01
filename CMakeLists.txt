cmake_minimum_required(VERSION 4.0)
project(ROOT)

# Enable export of compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/Catch2)

enable_testing()
include(CTest)
include(Catch)

add_subdirectory(ax25)

add_subdirectory(server)