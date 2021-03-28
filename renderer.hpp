#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

class Renderer {
    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        SDL_Texture* texture_;
    public:
        Renderer(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
        ~Renderer();

        void update(const void* buffer, int pitch);
        bool processInput(uint8_t* keys);
};

#endif
