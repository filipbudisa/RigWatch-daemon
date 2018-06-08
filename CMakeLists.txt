cmake_minimum_required(VERSION 3.5)
project(RigWatch-daemon)

set(CMAKE_C_STANDARD 11)

add_executable(RigWatch-daemon rigwatchd/rigwatchd.c rigwatchd/config.c rigwatchd/miner/claymore.c rigwatchd/connection.c)
target_link_libraries(RigWatch-daemon sensors dl)