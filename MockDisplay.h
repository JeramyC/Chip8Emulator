#pragma once
#include "IDisplay.h"
#include <iostream>

class MockDisplay : public IDisplay {
public:
    void clear() override {
        std::cout << "[MockDisplay] clear()\n";
    }

    bool drawSprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t numBytes) override {
        std::cout << "[MockDisplay] drawSprite(" << (int)x << ", " << (int)y << ", " << (int)numBytes << ")\n";
        return false; // no collision
    }
}; 
