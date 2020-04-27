#/usr/bin/env bash

alias first-build='(mkdir -p build && cd build && cmake .. && make)'
alias build='(cd build && make)'
alias emu=./build/emulator
alias asm=./build/disassembler