cmake_minimum_required(VERSION 4.0)
project(server)

find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)

# Set cpp standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(server 
    src/main.cpp
)

target_link_libraries(server
    PRIVATE
        ax25
        spdlog::spdlog
        OpenSSL::SSL
        OpenSSL::Crypto
)

target_include_directories(server
    PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/websocketpp
)