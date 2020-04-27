#include <algorithm>
#include <iostream>
#include <chip8/chip8-disassembler.hpp>

int main(int argc, const char **argv) {
  (void) argc;
  (void) argv;

  auto fileName = "c8games/MAZE";

  if (argc > 1) {
    fileName = argv[1];
  }

  chip8::disassembleProgram(fileName, std::cout);
  return 0;
}
