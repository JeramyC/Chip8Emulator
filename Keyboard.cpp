#include "Keyboard.h"

Keyboard::Keyboard() {
    keys.fill(0);
}

void Keyboard::pollEvents(bool& quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        else if (e.type == SDL_KEYDOWN) {
            int mapped = mapSDLKey(e.key.keysym.sym);
            if (mapped != -1) keys[mapped] = 1;
        }
        else if (e.type == SDL_KEYUP) {
            int mapped = mapSDLKey(e.key.keysym.sym);
            if (mapped != -1) keys[mapped] = 0;
        }
    }
}

bool Keyboard::isPressed(int key) {
    return keys[key] != 0;
}

int Keyboard::waitForKeyPress() {
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                int mapped = mapSDLKey(e.key.keysym.sym);
                if (mapped != -1) {
                    return mapped;
                }
            }
        }
    }
    return -1;
}

// Typical CHIP-8 keypad layout:
// 1 2 3 C
// 4 5 6 D
// 7 8 9 E
// A 0 B F
int Keyboard::mapSDLKey(SDL_Keycode sdlKey) {
    switch (sdlKey) {
    case SDLK_1: return 0x1;
    case SDLK_2: return 0x2;
    case SDLK_3: return 0x3;
    case SDLK_4: return 0xC;
    case SDLK_q: return 0x4;
    case SDLK_w: return 0x5;
    case SDLK_e: return 0x6;
    case SDLK_r: return 0xD;
    case SDLK_a: return 0x7;
    case SDLK_s: return 0x8;
    case SDLK_d: return 0x9;
    case SDLK_f: return 0xE;
    case SDLK_z: return 0xA;
    case SDLK_x: return 0x0;
    case SDLK_c: return 0xB;
    case SDLK_v: return 0xF;
    default: return -1;
    }
}
