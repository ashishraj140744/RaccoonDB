cmake_minimum_required(VERSION 3.16)
project(RaccoonDB)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(raccoondb
    program/main.cpp
    program/command_line/CommandLine.cpp
)
target_include_directories(raccoondb PRIVATE
    program
)