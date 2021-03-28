#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

class Platform {
    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        SDL_Texture* texture_;
    public:
        Platform(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
        ~Platform();

        void update(const void* buffer, int pitch);
        bool processInput(uint8_t* keys);
};

#endif
