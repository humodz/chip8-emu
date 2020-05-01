#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <boost/format.hpp>

#include <chip8/chip8-disassembler.hpp>

using boost::format;

#define FMT_REG "V%X"
#define FMT_VAL(L) "0x%0" #L "X"

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
      os << format("[" FMT_VAL(4) "] ") % (pc + 0x200);
      printInstruction(opCode, os);
      os << "\n";
    }
}

void chip8::printInstruction(uint16_t opCode, std::ostream &os) {
  os << format(FMT_VAL(4) ": ") % opCode;

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
      os << format("GOTO " FMT_VAL(3)) % address;
      break;
    }
    case 0x2: {
      os << format("CALL " FMT_VAL(3)) % address;
      break;
    }
    case 0x3: {
      os << format("SKIP.EQ " FMT_REG " " FMT_VAL(2)) % vx % immediate;
      break;
    }
    case 0x4: {
      os << format("SKIP.NE " FMT_REG " " FMT_VAL(2)) % vx % immediate;
      break;
    }
    case 0x5: {
      os << format("SKIP.EQ " FMT_REG " " FMT_VAL(2)) % vx % immediate;
      break;
    }
    case 0x6: {
      os << format("SET " FMT_REG " " FMT_VAL(2)) % vx % immediate;
      break;
    }
    case 0x7: {
      os << format("ADD " FMT_REG " " FMT_VAL(2)) % vx % immediate;
      break;
    }
    case 0x8: {
      switch (mathOp) {
        case 0x0: {
          os << format("SET " FMT_REG " " FMT_REG) % vx % vy;
          break;
        }
        case 0x1: {
          os << format("OR " FMT_REG " " FMT_REG) % vx % vy;
          break;
        }
        case 0x2: {
          os << format("AND " FMT_REG " " FMT_REG) % vx % vy;
          break;
        }
        case 0x3: {
          os << format("XOR " FMT_REG " " FMT_REG) % vx % vy;
          break;
        }
        case 0x4: {
          os << format("ADD " FMT_REG " " FMT_REG) % vx % vy;
          break;
        }
        case 0x5: {
          os << format("SUB " FMT_REG " " FMT_REG) % vx % vy;
          break;
        }
        case 0x6: {
          os << format("RSHIFT " FMT_REG) % vx;
          break;
        }
        case 0x7: {
          os << format("REVSUB " FMT_REG " " FMT_REG) % vx % vy;
          break;
        }
        case 0xE: {
          os << format("LSHIFT " FMT_REG) % vx;
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
      os << format("SKIP.NE " FMT_REG " " FMT_REG) % vx % vy;
      break;
    }
    case 0xA: {
      os << format("SET.INDEX " FMT_VAL(3)) % address;
      break;
    }
    case 0xB: {
      os << format("GOTO.V0 " FMT_VAL(3)) % address;
      break;
    }
    case 0xC: {
      os << format("RAND " FMT_REG " " FMT_VAL(2)) % vx % immediate;
      break;
    }
    case 0xD: {
      os << format("DRAW " FMT_REG " " FMT_REG " " FMT_VAL(1)) % vx % vy % spriteH;
      break;
    }
    case 0xE: {
      switch(opCode & 0x00FF) {
        case 0x009E: {
          os << format("SKIP.KEY.EQ " FMT_REG) % vx;
          break;
        }
        case 0x00A1: {
          os << format("SKIP.KEY.NE " FMT_REG) % vx;
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
        case 0x0007: {
          os << format("GET.DELAY " FMT_REG) % vx;
          break;
        }
        case 0x000A: {
          os << format("WAIT.KEY " FMT_REG) % vx;
          break;
        }
        case 0x0015: {
          os << format("SET.DELAY " FMT_REG) % vx;
          break;
        }
        case 0x0018: {
          os << format("SET.SOUND " FMT_REG) % vx;
          break;
        }
        case 0x001E: {
          os << format("ADD.INDEX " FMT_REG) % vx;
          break;
        }
        case 0x0029: {
          os << format("CHAR " FMT_REG) % vx;
          break;
        }
        case 0x0033: {
          os << format("BCD " FMT_REG) % vx;
          break;
        }
        case 0x0055: {
          os << format("DUMP V0.." FMT_REG) % vx;
          break;
        }
        case 0x0065: {
          os << format("LOAD V0.." FMT_REG) % vx;
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
