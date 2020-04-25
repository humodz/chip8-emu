#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <sstream>

#include "chip8-disassembler.h"
#include "util.h"

void chip8::disassembleProgram(const std::string &filePath, std::ostream &os) {
    std::ifstream fs{filePath.data(), std::ios_base::binary};

    if (!fs.good()) {
      std::stringstream builder;
      builder << "Could not open file: " << filePath;
      throw std::runtime_error(builder.str());
    }

    fs.seekg(0, std::ios_base::end);
    int programSize = fs.tellg();

    std::vector<uint8_t> memory(programSize, 0);

    fs.seekg(0, std::ios_base::beg);
    fs.read((char *) memory.data(), programSize);

    for (int pc = 0; pc < programSize; pc += 2) {
      uint16_t opCode = (memory[pc] << 8) | memory[pc + 1];
      os << "[" << util::formatHex(pc + 0x200, 4) << "] ";
      printInstruction(opCode, os);
      os << "\n";
    }
}

void chip8::printInstruction(uint16_t opCode, std::ostream &os) {
  os << util::formatHex(opCode, 4) << ": ";

  int vx = (opCode & 0x0F00) >> 8;
  int vy = (opCode & 0x00F0) >> 4;
  int address = opCode & 0x0FFF;
  int immediate = opCode & 0x00FF;
  int mathOp = opCode & 0x000F;
  int spriteH = opCode & 0x000F;

  switch((opCode & 0xF000) >> 12) {
    case 0x0: {
      switch (immediate) {
        case 0x00E0: {
          os << "CLEAR";
          break;
        }
        case 0x00EE: {
          os << "RETURN";
          break;
        }
        default: {
          os << "Unrecognized 0x0 instruction";
          break;
        }
      }
      break;
    }
    case 0x1: {
      os << "GOTO " << util::formatHex(address, 3);
      break;
    }
    case 0x2: {
      os << "CALL " << util::formatHex(address, 3); 
      break;
    }
    case 0x3: {
      os << "SKIP.EQ V" << util::formatHex(vx, 0, false) << " " << util::formatHex(immediate, 2);
      break;
    }
    case 0x4: {
      os << "SKIP.NE V" << util::formatHex(vx, 0, false) << " " << util::formatHex(immediate, 2);
      break;
    }
    case 0x6: {
      os << "SET V" << util::formatHex(vx, 0, false) << " " << util::formatHex(immediate, 2);
      break;
    }
    case 0x7: {
      os << "ADD V" << util::formatHex(vx, 0, false) << " " << util::formatHex(immediate, 2);
      break;
    }
    case 0x8: {
      switch (mathOp) {
        case 0x0: {
          os << "SETR V" << util::formatHex(vx, 0, false) << " V" << util::formatHex(vy, 0, false);
          break;
        }
        case 0x2: {
          os << "BAND V" << util::formatHex(vx, 0, false) << " V" << util::formatHex(vy, 0, false);
          break;
        }
        case 0x4: {
          os << "ADD V" << util::formatHex(vx, 0, false) << " V" << util::formatHex(vy, 0, false);
          break;
        }
        case 0x5: {
          os << "SUB V" << util::formatHex(vx, 0, false) << " V" << util::formatHex(vy, 0, false);
          break;
        }
        case 0x6: {
          os << "RSHIFT V" << util::formatHex(vx, 0, false);
          break;
        }
        case 0xE: {
          os << "LSHIFT V" << util::formatHex(vx, 0, false);
          break;
        }
        default: {
          os << "Unrecognized 0x8 instruction";
          break;
        }
      }
      break;
    }
    case 0x9: {
      os << "SKIP.RNE V" << util::formatHex(vx, 0, false) << " V" << util::formatHex(vy, 0, false);
      break;
    }
    case 0xA: {
      os << "SET.INDEX " << util::formatHex(address, 3);
      break;
    }
    case 0xC: {
      os << "RAND V" << util::formatHex(vx, 0, false) << " " << util::formatHex(immediate, 2);
      break;
    }
    case 0xD: {
      os << "DRAW V" << util::formatHex(vx, 0, false)
         << " V" << util::formatHex(vy, 0, false)
         << " " << util::formatHex(spriteH);
      break;
    }
    case 0xE: {
      switch(opCode & 0x00FF) {
        case 0x009E: {
          os << "SKIP.KEY.EQ V" << util::formatHex(vx, 0, false);
          break;
        }
        case 0x00A1: {
          os << "SKIP.KEY.NE V" << util::formatHex(vx, 0, false);
          break;
        }
        default: {
          os << "Unrecognized 0xE instruction";
          break;
        }
      }
      break;
    }
    case 0xF: {
      switch(opCode & 0x00FF) {
        case 0x000A: {
          os << "WAIT.KEY V" << util::formatHex(vx, 0, false);
          break;
        }
        case 0x0007: {
          os << "GET.DELAY V" << util::formatHex(vx, 0, false);
          break;
        }
        case 0x0015: {
          os << "SET.DELAY V" << util::formatHex(vx, 0, false);
          break;
        }
        case 0x0018: {
          os << "SET.SOUND V" << util::formatHex(vx, 0, false);
          break;
        }
        case 0x001E: {
          os << "ADD.INDEX V" << util::formatHex(vx, 0, false);
          break;
        }
        case 0x0029: {
          os << "CHAR V" << util::formatHex(vx, 0, false);
          break;
        }
        case 0x0033: {
          os << "BCD V" << util::formatHex(vx, 0, false);
          break;
        }
        case 0x0055: {
          os << "DUMP V0.." << util::formatHex(vx, 0, false);
          break;
        }
        case 0x0065: {
          os << "LOAD V0.." << util::formatHex(vx, 0, false);
          break;
        }
        default: {
          os << "Unrecognized 0xF instruction";
          break;
        }
      }
      break;
    }
    default: {
      os << "Unrecognized instruction";
      break;
    }
  }
}
