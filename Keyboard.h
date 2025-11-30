#pragma once
#include <SDL2/SDL.h>
#include <array>

class Keyboard {
public:
    Keyboard();
    void pollEvents(bool& quit);
    bool isPressed(int key);
    int waitForKeyPress();

private:
    std::array<uint8_t, 16> keys;
    int mapSDLKey(SDL_Keycode sdlKey);
};