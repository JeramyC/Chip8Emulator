#include "Memory.h"
#include <fstream>
#include <iostream>
#include <vector>

static const unsigned char chip8_fontset[80] = { //fontset to build characters on screen

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

Memory::Memory() {      //Memory constructor, SIZE is 4096, function zeroes out memory and calls the loadFontset function
    for (int i = 0; i < SIZE; i++) memory[i] = 0;  
    loadFontset();
    }

void Memory::loadFontset() {        //loads the fontset into memory
    for (int i = 0; i < 80; i++) memory[i] = chip8_fontset[i];
}

bool Memory::loadROM(const std::string &path) {  //load rom function
    std::ifstream rom(path, std::ios::binary | std::ios::ate);
    if (!rom) {
        std::cerr << "Failed to open ROM: " << path << "\n";
        return false;
    }

    std::streamsize size = rom.tellg();  //gets the ROM size and sets the file pointer back to the start
    rom.seekg(0, std::ios::beg);

    if (size > (SIZE - 512)) {      //if size of the ROM is too large, then fails
        std::cerr << "ROM too large\n";
        return false;
    }

    std::vector<char> buffer(size);  //reads the rom into temporary buffer and then copies into memory at 0x200
    if (!rom.read(buffer.data(), size)) return false;

    for (int i = 0; i < size; i++) {
        memory[i + 512] = static_cast<uint8_t>(buffer[i]);
    }

    return true;
}

