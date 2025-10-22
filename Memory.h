#pragma once
#include <cstdint>
#include <string>

class Memory {
public:
	static const int SIZE = 4096;  //CHIP-8 has 4096 bytes of memory
	Memory();

	void loadFontset();
	bool loadROM(const std::string &path);
	uint8_t &operator[](int index) { return memory[index]; }

private:
	uint8_t memory[SIZE];

};