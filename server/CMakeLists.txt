cmake_minimum_required(VERSION 4.0)
project(server)

# Set cpp standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(server 
    src/main.cpp
)

target_link_libraries(server ax25 spdlog::spdlog)