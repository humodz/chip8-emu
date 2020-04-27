cmake_minimum_required (VERSION 3.0)
project (CHIP8)

find_package(Boost 1.65)
find_package (SDL2 REQUIRED)
set (SDL2EXTRA_LIBRARIES "-lSDL2_image -lSDL2_ttf")

set (PROGRAMS
  hello-boost
  hello-sdl
  emulator
  disassembler
)

set (LIBCHIP8_FILES 
  src/chip8/chip8-disassembler.cpp
  src/chip8/chip8-emu.cpp
)

set (COMPILER_OPTIONS -Wall -Wextra -pedantic)

add_library (chip8 STATIC ${LIBCHIP8_FILES})
target_include_directories (chip8 PUBLIC src)
target_compile_options (chip8 PRIVATE ${COMPILER_OPTIONS})

foreach (program ${PROGRAMS})
  add_executable (${program} src/programs/${program}.cpp)
  target_link_libraries (${program} chip8)
  target_compile_options (${program} PRIVATE ${COMPILER_OPTIONS})

  target_include_directories(${program} PRIVATE src)
  target_link_libraries(${program} ${BOOST_LIBRARIES})

  target_include_directories(${program} PRIVATE ${SDL2_INCLUDE_DIRS})
  target_link_libraries(${program} ${SDL2_LIBRARIES})
  target_link_libraries(${program} ${SDL2EXTRA_LIBRARIES})
endforeach (program)