#pragma once
#include <cstdint>
#include <string>

class Memory {
public:
	static const int SIZE = 4096;  // CHIP-8 has 4096 bytes of memory

	Memory();                                   // Constructs and initializes the memory
	void loadFontset();                         // Loads the Chip-8 fontset into memory
	bool loadROM(const std::string& path);      // Loads a ROM from file into memory
	uint8_t& operator[](int index) { return memory[index]; } // Gets reference to byte at index

private:
	uint8_t memory[SIZE];
};