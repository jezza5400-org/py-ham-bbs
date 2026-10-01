cmake_minimum_required(VERSION 4.0)
project(ax25)

# Set cpp standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(ax25 
    src/ax25/ax25.cpp src/ax25/ax25.h
)
target_link_libraries(ax25 spdlog::spdlog)
target_include_directories(ax25 PUBLIC src)