#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <boost/format.hpp>

#include <chip8/chip8-emu.hpp>
#include <chip8/chip8-disassembler.hpp>

chip8::Chip8Emu::Chip8Emu() {
  hardReset();
}

namespace {
  void throwIfProgramTooBig(size_t programSize) {
    size_t maxFileSize = chip8::MEMORY_SIZE - chip8::PROGRAM_OFFSET;

    if (programSize > maxFileSize) {
      std::stringstream builder;
      builder << "Program is too big: " << programSize << "B. Maximum is " << maxFileSize << "B";
      throw std::runtime_error(builder.str());
    }
  }
}

void chip8::Chip8Emu::hardReset() {
  state = EmulationState::RUNNING;
  opCode = 0;
  std::fill(std::begin(memory), std::end(memory), 0);
  std::fill(std::begin(V), std::end(V), 0);
  I = 0;
  pc = PROGRAM_OFFSET;
  std::fill(std::begin(gfx), std::end(gfx), 0);
  delayTimer = 0;
  soundTimer = 0;
  std::fill(std::begin(stack), std::end(stack), 0);
  sp = 0;
  std::fill(std::begin(key), std::end(key), 0);
  drawFlag = false;
}

void chip8::Chip8Emu::loadProgramFromFile(const std::string &filePath) {
  std::ifstream fs{filePath.data(), std::ios_base::binary};

  if (!fs.good()) {
    std::stringstream builder;
    builder << "Could not open file: " << filePath;
    throw std::runtime_error(builder.str());
  }

  fs.seekg(0, std::ios_base::end);
  size_t programSize = fs.tellg();
  fs.seekg(0, std::ios_base::beg);

  throwIfProgramTooBig(programSize);
  fs.read((char *) &memory[PROGRAM_OFFSET], programSize);
}

void chip8::Chip8Emu::loadProgramFromBuffer(const uint8_t *buffer, size_t bufferSize) {
  throwIfProgramTooBig(bufferSize);
  std::copy(&buffer[0], &buffer[bufferSize], &memory[PROGRAM_OFFSET]);
}

void chip8::Chip8Emu::loadFont(const uint8_t *buffer, size_t bufferSize) {
  std::copy(&buffer[0], &buffer[bufferSize], &memory[0]);
}

void chip8::Chip8Emu::drawScreen(std::ostream &os) {
  //os << "\e[1;1H\e[2J" << std::endl;
  
  for (int i = 0; i < GFX_WIDTH; i++) {
    os << '=';
  }
  os << std::endl;

  for (int i = 0; i < GFX_HEIGHT; i++) {
    for (int j = 0; j < GFX_WIDTH; j++) {
      os << (gfx[GFX_WIDTH * i + j] ? 'X' : ' ');
    }
    os << std::endl;
  }

  for (int i = 0; i < GFX_WIDTH; i++) {
      os << '=';
  }
  os << std::endl;
}

uint16_t chip8::Chip8Emu::fetch(uint16_t address) {
  return (memory[address] << 8) | memory[address + 1]; 
}

void chip8::Chip8Emu::updateTimers() {
  if (delayTimer > 0) {
    delayTimer--;
  }

  if (soundTimer > 0) {
    soundTimer--;
  }
}

void chip8::Chip8Emu::emulateCycle(int keyEvent, std::ostream *os) {
  using boost::format;
  if (!isRunning()) {
    return;
  }

  drawFlag = false;
  opCode = fetch(pc);
  bool incrementPc = true;

  if (os != nullptr) {
    (*os) << format("[0x%04X] ") % pc;
    printInstruction(opCode, *os);
  }

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
          std::fill(std::begin(gfx), std::end(gfx), 0);
          lastDrawX = 0;
          lastDrawY = 0;
          lastDrawW = GFX_WIDTH;
          lastDrawH = GFX_HEIGHT;
          break;
        }
        case 0x00EE: {
          if (sp == 0) {
            state = EmulationState::STOPPED_STACK_UNDERFLOW;
            return;
          }  

          sp--;
          pc = stack[sp];
          break;
        }
        default: {
          state = EmulationState::STOPPED_UNRECOGNIZED_OPCODE;
          return;
          break;
        }
      }
      break;
    }
    case 0x1: {
      if (pc != address) {
        pc = address;
        incrementPc = false;
      } else {
        state = EmulationState::STOPPED_INFINITE_GOTO;
        return;
      }
      break;
    }
    case 0x2: {
      if (sp == STACK_SIZE) {
        state = EmulationState::STOPPED_STACK_OVERFLOW;
        return;
      }

      stack[sp] = pc;
      sp++;
      pc = address;
      incrementPc = false;
      break;
    }
    case 0x3: {
      if (V[vx] == immediate) {
        pc += 2;
      }
      break;
    }
    case 0x4: {
      if (V[vx] != immediate) {
        pc += 2;
      }
      break;
    }
    case 0x6: {
      V[vx] = immediate;
      break;
    }
    case 0x7: {
      V[vx] += immediate;
      break;
    }
    case 0x8: {
      switch (mathOp) {
        case 0x0: {
          V[vx] = V[vy];
          break;
        }
        case 0x2: {
          V[vx] = V[vx] & V[vy];
          break;
        }
        case 0x4: {
          uint8_t oldVx = V[vx];
          V[vx] = V[vx] + V[vy];
          V[0xF] = (oldVx > V[vx]) ? 1 : 0;
          break;
        }
        case 0x5: {
          uint8_t oldVx = V[vx];
          V[vx] = V[vx] - V[vy];
          V[0xF] = (oldVx > V[vx]) ? 1 : 0;
          // V[0xF] = (oldVx < V[vx]) ? 1 : 0;
          break;
        }
        case 0x6: {
          V[0xF] = V[vx] & 1;
          V[vx] = V[vx] >> 1;
          break;
        }
        case 0xE: {
          V[0xF] = (V[vx] >> 15) & 1;
          V[vx] = V[vx] << 1;
          break;
        }
        default: {
          state = EmulationState::STOPPED_UNRECOGNIZED_OPCODE;
          return;
          break;
        }
      }
      break;
    }
    case 0x9: {
      if (V[vx] != V[vy]) {
        pc += 2;
      }
      break;
    }
    case 0xA: {
      I = address;
      break;
    }
    case 0xC: {
      V[vx] = rand() & immediate;
      break;
    }
    case 0xD: {
      drawFlag = true;
      bool collided = false;

      lastDrawX = V[vx];
      lastDrawY = V[vy];
      lastDrawW = 8;
      lastDrawH = spriteH;

      for (int i = 0; i < spriteH; i++) {
        int byte = memory[I + i];
        for (int j = 0; j < 8; j++) {
          int bit = (byte >> (7 - j)) & 1;

          int px = (V[vx] + j) % GFX_WIDTH;
          int py = (V[vy] + i) % GFX_HEIGHT;

          if (px < GFX_WIDTH && py < GFX_HEIGHT) {
            int index = GFX_WIDTH * py + px;
            int oldValue = gfx[index];
            gfx[index] = gfx[index] != bit;

            if (oldValue && !gfx[index]) {
              collided = true;
            }
          }
        }
      }

      V[0xF] = collided ? 1 : 0;
      break;
    }
    case 0xE: {
      switch(opCode & 0x00FF) {
        case 0x009E: {
          if(V[vx] >= NUM_KEYS) {
            state = EmulationState::STOPPED_SEGFAULT;
            return;
          }
          if (key[V[vx]]) {
            pc += 2;
          }
          break;
        }
        case 0x00A1: {
          if(V[vx] >= NUM_KEYS) {
            state = EmulationState::STOPPED_SEGFAULT;
            return;
          }
          if (!key[V[vx]]) {
            pc += 2;
          }
          break;
        }
        default: {
          state = EmulationState::STOPPED_UNRECOGNIZED_OPCODE;
          break;
        }
      }
      break;
    }
    case 0xF: {
      switch(immediate) {
        case 0x000A: {
          if (keyEvent == -1) {
            state = EmulationState::RUNNING_WAITING_INPUT;
            incrementPc = false;
          } else {
            state = EmulationState::RUNNING;
            V[vx] = keyEvent;
          }
          break;
        }
        case 0x0007: {
          V[vx] = delayTimer;
          break;
        }
        case 0x0015: {
          delayTimer = V[vx];
          break;
        }
        case 0x0018: {
          soundTimer = V[vx];
          break;
        }
        case 0x001E: {
          V[0xF] = (I + V[vx]) > I ? 0 : 1;
          I = I + V[vx];
          break;
        }
        case 0x0029: {
          I = defaultCharSizeBytes * V[vx];
          break;
        }
        case 0x0033: {
          if (I + 2 < 0 || I + 2 > MEMORY_SIZE) {
            state = EmulationState::STOPPED_SEGFAULT;
            return;
          }

          memory[I + 0] = (V[vx] / 100) % 10;
          memory[I + 1] = (V[vx] /  10) % 10;
          memory[I + 2] = (V[vx]      ) % 10;
          break;
        }
        case 0x0055: {
          for (int i = 0; i <= vx; i++) {
            if (I + i < 0 || I + i + 1 > MEMORY_SIZE) {
              state = EmulationState::STOPPED_SEGFAULT;
              return;
            }

            memory[I + i] = V[i];
          }
          break;
        }
        case 0x0065: {
          for (int i = 0; i <= vx; i++) {
            if (I + i < 0 || I + i > MEMORY_SIZE) {
              state = EmulationState::STOPPED_SEGFAULT;
              return;
            }

            V[i] = memory[I + i];
          }
          break;
        }
        default: {
          state = EmulationState::STOPPED_UNRECOGNIZED_OPCODE;
          return;
          break;
        }
      }
      break;
    }
    default: {
      state = EmulationState::STOPPED_UNRECOGNIZED_OPCODE;
      return;
      break;
    }
  }

  if (incrementPc) {
    pc += 2;
  }

  if (pc > MEMORY_SIZE) {
    state = EmulationState::STOPPED_INVALID_PC;
  }
}