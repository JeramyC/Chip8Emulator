#pragma once
#include <cstdint>
#include "Memory.h"
#include "Display.h"
#include "Keyboard.h"

class CPU {
public:
    CPU(Memory& mem, Display& disp, Keyboard& kbd);

    void init();
    void cycle();

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

    void executeOpcode();
};