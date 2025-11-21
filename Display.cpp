#include "Display.h"
#include <iostream>

Display::Display() {      // Constructs display and initializes SDL window and renderer
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    window = SDL_CreateWindow("Chip-8 Emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH * SCALE,
        HEIGHT * SCALE,
        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    clear();
}

Display::~Display() {     // Cleans up SDL window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::clear() {   // Clears display pixels and updates renderer to black
    pixels.fill(0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

bool Display::drawSprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t height) {
    // Draws a sprite on the display, returns true if any pixels are erased (collision)
    bool pixelErased = false;

    for (int row = 0; row < height; row++) {
        uint8_t byte = sprite[row];
        for (int col = 0; col < 8; col++) {
            if ((byte & (0x80 >> col)) != 0) {
                int px = (x + col) % WIDTH;
                int py = (y + row) % HEIGHT;
                int idx = px + py * WIDTH;

                if (pixels[idx] == 1) {
                    pixelErased = true;
                }
                pixels[idx] ^= 1;
            }
        }
    }

    return pixelErased;
}

void Display::render() {       // Renders the current pixel buffer to the window
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (pixels[x + y * WIDTH]) {
                SDL_Rect rect = { x * SCALE, y * SCALE, SCALE, SCALE };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}