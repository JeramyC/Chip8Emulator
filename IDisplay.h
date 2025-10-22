#pragma once
#include <cstdint>

class IDisplay {
public:
    virtual ~IDisplay() = default;
    virtual void clear() = 0;
    virtual bool drawSprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t numBytes) = 0;
};