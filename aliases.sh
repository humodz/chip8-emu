#/usr/bin/env bash

alias first-build='(mkdir -p build && cd build && cmake .. && make)'
alias build='(cd build && make)'
alias emu=./build/src/programs/emulator/emulator
alias asm=./build/src/programs/disassembler/disassembler