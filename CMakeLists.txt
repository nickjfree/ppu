cmake_minimum_required(VERSION 2.8.9)
project (ppu)

include_directories(include)

file(GLOB_RECURSE sources src/*.cpp)

add_executable(ppu ${sources})