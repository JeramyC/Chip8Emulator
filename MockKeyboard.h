#pragma once
#include "IKeyboard.h"
#include <iostream>

class MockKeyboard : public IKeyboard {
public:
    bool isPressed(uint8_t key) override {
        std::cout << "[MockKeyboard] isPressed(" << (int)key << ")\n";
        return false;
    }

    int waitForKeyPress() override {
        std::cout << "[MockKeyboard] waitForKeyPress() -> returning -1\n";
        return -1; // no input
    }
}; 
