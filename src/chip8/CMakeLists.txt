add_library(chip8 STATIC
  chip8-emu.cpp
  chip8-disassembler.cpp
)

target_link_libraries(chip8 ${BOOST_LIBRARIES})