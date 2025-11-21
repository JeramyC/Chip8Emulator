#include "Memory.h"
#include <fstream>
#include <iostream>
#include <vector>

static const unsigned char chip8_fontset[80] = { // fontset to build characters on screen

    0xF0,0x90,0x90,0x90,0xF0, // 0
    0x20,0x60,0x20,0x20,0x70, // 1
    0xF0,0x10,0xF0,0x80,0xF0, // 2
    0xF0,0x10,0xF0,0x10,0xF0, // 3
    0x90,0x90,0xF0,0x10,0x10, // 4
    0xF0,0x80,0xF0,0x10,0xF0, // 5
    0xF0,0x80,0xF0,0x90,0xF0, // 6
    0xF0,0x10,0x20,0x40,0x40, // 7
    0xF0,0x90,0xF0,0x90,0xF0, // 8
    0xF0,0x90,0xF0,0x10,0xF0, // 9
    0xF0,0x90,0xF0,0x90,0x90, // A
    0xE0,0x90,0xE0,0x90,0xE0, // B
    0xF0,0x80,0x80,0x80,0xF0, // C
    0xE0,0x90,0x90,0x90,0xE0, // D
    0xF0,0x80,0xF0,0x80,0xF0, // E
    0xF0,0x80,0xF0,0x80,0x80  // F
};

Memory::Memory() {      // Constructs and initializes 4096 bytes, then loads fontset
    for (int i = 0; i < SIZE; i++) memory[i] = 0;
    loadFontset();
}

void Memory::loadFontset() {        // Loads the fontset into the beginning of memory
    for (int i = 0; i < 80; i++) memory[i] = chip8_fontset[i];
}

bool Memory::loadROM(const std::string& path) {  // Loads a ROM file into memory at 0x200
    std::ifstream rom(path, std::ios::binary | std::ios::ate);
    if (!rom) {
        std::cerr << "Failed to open ROM: " << path << "\n";
        return false;
    }

    std::streamsize size = rom.tellg();  // Gets ROM file size and resets pointer
    rom.seekg(0, std::ios::beg);

    if (size > (SIZE - 512)) {      // If ROM is too large, fails
        std::cerr << "ROM too large\n";
        return false;
    }

    std::vector<char> buffer(size);  // Reads ROM to temp buffer and copies to memory at 0x200
    if (!rom.read(buffer.data(), size)) return false;

    for (int i = 0; i < size; i++) {
        memory[i + 512] = static_cast<uint8_t>(buffer[i]);
    }

    return true;
}