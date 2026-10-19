cmake_minimum_required(VERSION 3.20)
project(chip8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

add_executable(chip8 src/main.cpp)
target_link_libraries(chip8 PRIVATE SDL2::SDL2)
