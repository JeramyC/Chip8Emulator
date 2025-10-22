#pragma once
#include <SDL2/SDL.h>
#include <array>
#include <cstdint>

class Display {
public:
    static const int WIDTH = 64;
    static const int HEIGHT = 32;
    static const int SCALE = 10; // Each CHIP-8 pixel will be scaled up

    Display();
    ~Display();

    void clear();
    bool drawSprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t height);
    void render();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::array<uint8_t, WIDTH* HEIGHT> pixels;
};
