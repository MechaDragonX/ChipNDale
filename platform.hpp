#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <SDL2/SDL.h>
// #include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

class Platform {
    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        SDL_Texture* texture_;
    public:
        Platform(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
        ~Platform();

        void update();
        void processInput();
};

#endif
