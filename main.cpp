#include "Memory.h"
#include "CPU.h"
#include "Display.h"
#include "Keyboard.h"
#include <chrono>
#include <thread>
#include <iostream>

int main(int argc, char* argv[]) {
    // Entry point: sets up emulator, loads ROM, and runs the emulation loop
    if (argc < 2) {
        std::cerr << "Usage: ./chip8 <ROM file>\n";
        return 1;
    }

    Memory memory;
    Display display;
    Keyboard keyboard;
    CPU cpu(memory, display, keyboard);

    if (!memory.loadROM(argv[1])) {
        std::cerr << "Failed to load ROM\n";
        return 1;
    }

    bool quit = false;

    while (!quit) {
        cpu.cycle();             // Runs one CPU cycle
        display.render();        // Draws display
        keyboard.pollEvents(quit); // Handles input

        // 60Hz timer (approx 16ms per cycle)
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    return 0;
}