#pragma once
#include <cstdint>
#include "Memory.h"
#include "Display.h"
#include "Keyboard.h"

class CPU {
public:
    CPU(Memory &mem, Display &disp, Keyboard &kbd);   // Constructs a CPU and attaches key hardware components

    void init();                // Initializes the CPU and resets registers/state
    void cycle();               // Runs one emulation cycle (fetch, decode, execute)

private:
    Memory& memory;
    Display& display;
    Keyboard& keyboard;

    uint16_t opcode;
    uint16_t pc;
    uint16_t I;
    uint16_t sp;

    uint8_t V[16];
    uint16_t stack[16];
    uint8_t delay_timer;
    uint8_t sound_timer;

    void executeOpcode();       // Decodes and executes the current opcode
};
