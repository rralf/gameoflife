cmake_minimum_required(VERSION 2.8)

project(gol)
add_definitions(-std=c++11)

set(SRCS main.cpp)

add_executable(gol ${SRCS})
target_link_libraries(gol)
