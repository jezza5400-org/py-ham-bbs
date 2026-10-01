cmake_minimum_required(VERSION 4.0)
project(server)

# Set cpp standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(server 
    src/main.cpp
    src/ax25/ax25.cpp src/ax25/ax25.h
)
target_link_libraries(server spdlog::spdlog)