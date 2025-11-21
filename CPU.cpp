#include "CPU.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

CPU::CPU(Memory& mem, Display& disp, Keyboard& kbd)
    : memory(mem), display(disp), keyboard(kbd) {
    srand(static_cast<unsigned>(time(nullptr)));
    init();
}

void CPU::init() {
    pc = 0x200; // Programs start at 0x200
    opcode = 0;
    I = 0;
    sp = 0;

    for (int i = 0; i < 16; ++i) {
        V[i] = 0;
        stack[i] = 0;
    }

    delay_timer = 0;
    sound_timer = 0;

    display.clear();
}

void CPU::cycle() {
    opcode = memory[pc] << 8 | memory[pc + 1]; // Fetch op code, opcode is 16bits

    //std::cout << "Executing opcode: " << std::hex << opcode
    //    << " at PC=" << pc << std::dec << "\n";
    executeOpcode();

    if (delay_timer > 0) --delay_timer;
    if (sound_timer > 0) {
        if (--sound_timer == 0) {
            std::cout << "BEEP!\n";
        }
    }
}

void CPU::executeOpcode() {
    switch (opcode & 0xF000) {
    case 0x0000:
        switch (opcode & 0x00FF) {
        case 0x00E0: // Clear screen
            display.clear();
            pc += 2;
            break;
        case 0x00EE: // Return from subroutine
            --sp;
            pc = stack[sp];
            pc += 2;
            break;
        default:
            std::cerr << "Unknown opcode [0x0000]: " << std::hex << opcode << "\n";
            pc += 2;
            break;
        }
        break;

        // 1NNN - Jumps to address NNN
    case 0x1000:
        pc = opcode & 0x0FFF;
        break;

        // 2NNN - Calls subroutine at NNN
    case 0x2000:
        stack[sp] = pc;
        ++sp;
        pc = opcode & 0x0FFF;
        break;

        // 3XNN - Skips the next instruction if VX equals NN
    case 0x3000: {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = opcode & 0x00FF;
        pc += (V[x] == nn) ? 4 : 2;
        break;
    }
               // 4XNN - Skips the next instruction if VX does not equal NN.
    case 0x4000: { // SNE Vx, byte
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = opcode & 0x00FF;
        pc += (V[x] != nn) ? 4 : 2;
        break;
    }
               // 5XY0 - Skips the next instruction if VX equals VY
    case 0x5000: {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        pc += (V[x] == V[y]) ? 4 : 2;
        break;
    }
               // 6XNN - Sets VX to NN
    case 0x6000: {
        uint8_t x = (opcode & 0x0F00) >> 8;
        V[x] = opcode & 0x00FF;
        pc += 2;
        break;
    }
               // 7XNN - Adds NN to VX
    case 0x7000: {
        uint8_t x = (opcode & 0x0F00) >> 8;
        V[x] += opcode & 0x00FF;
        pc += 2;
        break;
    }

    case 0x8000: {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        switch (opcode & 0x000F) {
        case 0x0: V[x] = V[y]; break;  // 8XY0 - Sets VX to the value of VY
        case 0x1: V[x] |= V[y]; break; // 8XY1 - Sets VX to (VX OR VY)
        case 0x2: V[x] &= V[y]; break; // 8XY2 - Sets VX to (VX AND VY)
        case 0x3: V[x] ^= V[y]; break; // 8XY3 - Sets VX to (VX XOR VY)
            // 8XY4 - Adds VY to VX. VF is set to 1 when there's a carry,
            // and to 0 when there isn't.
        case 0x4: {
            uint16_t sum = V[x] + V[y];
            V[0xF] = (sum > 255);
            V[x] = sum & 0xFF;
            break;

        }
                // 8XY5 - VY is subtracted from VX. VF is set to 0 when
                // there's a borrow, and 1 when there isn't
        case 0x5:
            V[0xF] = V[x] > V[y];
            V[x] -= V[y];
            break;

            // 0x8XY6 - Shifts VX right by one. VF is set to the value of
            // the least significant bit of VX before the shift
        case 0x6:
            V[0xF] = V[x] & 1;
            V[x] >>= 1;
            break;
            // 0x8XY7: Sets VX to VY minus VX. VF is set to 0 when there's
            // a borrow, and 1 when there isn't
        case 0x7:
            V[0xF] = V[y] > V[x];
            V[x] = V[y] - V[x];
            break;
            // 0x8XYE: Shifts VX left by one. VF is set to the value of
           // the most significant bit of VX before the shift
        case 0xE:
            V[0xF] = (V[x] & 0x80) >> 7;
            V[x] <<= 1;
            break;
        default:
            std::cerr << "Unknown opcode [0x8000]: " << std::hex << opcode << "\n";
            break;
        }
        pc += 2;
        break;
    }
               // 9XY0 - Skips the next instruction if VX doesn't equal VY
    case 0x9000: {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        pc += (V[x] != V[y]) ? 4 : 2;
        break;
    }
               // ANNN - Sets I to the address NNN
    case 0xA000:
        I = opcode & 0x0FFF;
        pc += 2;
        break;
        // BNNN - Jumps to the address NNN plus V0
    case 0xB000:
        pc = (opcode & 0x0FFF) + V[0];
        break;
        // CXNN - Sets VX to a random number, masked by NN
    case 0xC000: { // RND Vx, byte
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = opcode & 0x00FF;
        V[x] = (rand() % 256) & nn;
        pc += 2;
        break;
    }

    case 0xD000: { // DRW Vx, Vy, nibble
        uint8_t x = V[(opcode & 0x0F00) >> 8];
        uint8_t y = V[(opcode & 0x00F0) >> 4];
        uint8_t height = opcode & 0x000F;
        V[0xF] = display.drawSprite(x, y, &memory[I], height);
        pc += 2;
        break;
    }

    case 0xE000: {
        uint8_t x = (opcode & 0x0F00) >> 8;
        switch (opcode & 0x00FF) {
        case 0x9E: // SKP Vx
            pc += (keyboard.isPressed(V[x])) ? 4 : 2;
            break;
        case 0xA1: // SKNP Vx
            pc += (!keyboard.isPressed(V[x])) ? 4 : 2;
            break;
        default:
            std::cerr << "Unknown opcode [0xE000]: " << std::hex << opcode << "\n";
            pc += 2;
            break;
        }
        break;
    }

    case 0xF000: {
        uint8_t x = (opcode & 0x0F00) >> 8;
        switch (opcode & 0x00FF) {
        case 0x07: V[x] = delay_timer; break;
        case 0x0A: {
            int key = keyboard.waitForKeyPress();
            if (key != -1) {
                V[x] = key;
            }
            else {
                return; // stall until key press
            }
            break;
        }
        case 0x15: delay_timer = V[x]; break;
        case 0x18: sound_timer = V[x]; break;
        case 0x1E: I += V[x]; break;
        case 0x29: I = V[x] * 5; break; // font sprite
        case 0x33: { // BCD
            memory[I] = V[x] / 100;
            memory[I + 1] = (V[x] / 10) % 10;
            memory[I + 2] = V[x] % 10;
            break;
        }
        case 0x55: // Store V0..Vx in memory
            for (int i = 0; i <= x; i++) memory[I + i] = V[i];
            break;
        case 0x65: // Load V0..Vx from memory
            for (int i = 0; i <= x; i++) V[i] = memory[I + i];
            break;
        default:
            std::cerr << "Unknown opcode [0xF000]: " << std::hex << opcode << "\n";
            break;
        }
        pc += 2;
        break;
    }

    default:
        std::cerr << "Unknown opcode: " << std::hex << opcode << "\n";
        pc += 2;
        break;
    }
}