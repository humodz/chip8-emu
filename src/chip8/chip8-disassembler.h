#ifndef CHIP8_DISASSEMBLER_H
#define CHIP8_DISASSEMBLER_H

#include <string>
#include <iostream>

namespace chip8 {
  void disassembleProgram(const std::string &filePath, std::ostream &os);
  void printInstruction(uint16_t opCode, std::ostream &os);
}

#endif
