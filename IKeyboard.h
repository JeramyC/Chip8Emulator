#pragma once
#include <cstdint>

class IKeyboard {
public:
    virtual ~IKeyboard() = default;
    virtual bool isPressed(uint8_t key) = 0;
    virtual int waitForKeyPress() = 0; // can return -1 if no key
};

