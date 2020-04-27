#ifndef CHIP8_EMU_HPP
#define CHIP8_EMU_HPP

#include <cstdint>
#include <string>
#include <iostream>

namespace chip8 {
  constexpr int PROGRAM_OFFSET = 0x200;
  constexpr int MEMORY_SIZE = 4096;
  constexpr int NUM_REGISTERS = 16;
  constexpr int GFX_WIDTH = 64; 
  constexpr int GFX_HEIGHT = 32;
  constexpr int STACK_SIZE = 16;
  constexpr int NUM_KEYS = 16;

  const uint8_t defaultFont[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80, // F
  };

  const size_t defaultFontSize = sizeof defaultFont;
  const size_t defaultCharSizeBytes = 5;

  enum class EmulationState {
    INVALID = 0,
    RUNNING = 1,
    RUNNING_WAITING_INPUT = (1 << 2) & 1,
    STOPPED = 1 << 1,
    STOPPED_UNRECOGNIZED_OPCODE = (1 << 2) | STOPPED,
    STOPPED_INFINITE_GOTO = (1 << 3) | STOPPED,
    STOPPED_INVALID_PC = (1 << 4) | STOPPED,
    STOPPED_STACK_UNDERFLOW = (1 << 5) | STOPPED,
    STOPPED_STACK_OVERFLOW = (1 << 6) | STOPPED,
    STOPPED_SEGFAULT = (1 << 7) | STOPPED
  };

  class Chip8Emu {
  public:
    EmulationState state;
    uint16_t opCode;
    uint8_t memory[MEMORY_SIZE];
    uint8_t V[NUM_REGISTERS]; // registers
    uint16_t I; // index pointer
    uint16_t pc; // program counter
    uint8_t gfx[GFX_HEIGHT * GFX_WIDTH]; // screen
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint16_t stack[STACK_SIZE];
    uint16_t sp; // stack pointer
    bool key[NUM_KEYS]; // pressed keys
    bool drawFlag;
    int lastDrawX;
    int lastDrawY;
    int lastDrawW;
    int lastDrawH;

    Chip8Emu();
    void hardReset();
    void loadProgramFromFile(const std::string &filePath);
    void loadProgramFromBuffer(const uint8_t *buffer, size_t bufferSize);
    void loadFont(const uint8_t *buffer, size_t bufferSize);
    void drawScreen(std::ostream &os);
    void updateTimers();
    void emulateCycle(int keyEvent, std::ostream *os = nullptr);
    void emulateCycle(int keyEvent, std::ostream &os) { emulateCycle(keyEvent, &os); }
    bool isRunning() {
      return state == EmulationState::RUNNING || state == EmulationState::RUNNING_WAITING_INPUT;
    }
    uint16_t fetch(uint16_t address);
  };
}

#endif