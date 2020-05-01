cmake_minimum_required(VERSION 3.0)
project(CHIP8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

find_package(Boost 1.65 REQUIRED)
find_package (SDL2 REQUIRED)
set(SDL2EXTRA_LIBRARIES "-lSDL2_image -lSDL2_ttf -lSDL2_mixer")

include_directories(include)

add_subdirectory(src/chip8)
add_subdirectory(src/gfx)
add_subdirectory(src/programs)