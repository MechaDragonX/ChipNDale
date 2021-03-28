#include <SDL2/SDL.h>
// #include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include "platform.hpp"

Platform::Platform(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight) {
    SDL_Init(SDL_INIT_VIDEO);
    window_ = SDL_CreateWindow(title, 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}
Platform::~Platform() {
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Platform::update(const void* buffer, int pitch) {
    SDL_UpdateTexture(texture_, nullptr, buffer, pitch);
    SDL_RenderClear(renderer_);
    SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
}
bool Platform::processInput(uint8_t* keys) {
    bool quit = false;

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN: {
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_x:
                        keys[0x00] = 1;
                        break;
                    case SDLK_1:
                        keys[0x01] = 1;
						break;
                    case SDLK_2:
                        keys[0x02] = 1;
						break;
                    case SDLK_3:
                        keys[0x03] = 1;
                        break;
                    case SDLK_q:
                        keys[0x04] = 1;
                        break;
                    case SDLK_w:
                        keys[0x05] = 1;
                        break;
                    case SDLK_e:
                        keys[0x06] = 1;
                        break;
                    case SDLK_a:
                        keys[0x07] = 1;
                        break;
                    case SDLK_s:
                        keys[0x08] = 1;
                        break;
                    case SDLK_d:
                        keys[0x09] = 1;
                        break;
                    case SDLK_z:
                        keys[0x0A] = 1;
                        break;
                    case SDLK_c:
                        keys[0x0B] = 1;
                        break;
                    case SDLK_4:
                        keys[0x0C] = 1;
                        break;
                    case SDLK_r:
                        keys[0x0D] = 1;
                        break;
                    case SDLK_f:
                        keys[0x0E] = 1;
                        break;
                    case SDLK_v:
                        keys[0x0F] = 1;
                        break;
                }
                break;
            }
            case SDL_KEYUP: {
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_x:
                        keys[0x00] = 0;
                        break;
                    case SDLK_1:
                        keys[0x01] = 0;
						break;
                    case SDLK_2:
                        keys[0x02] = 0;
						break;
                    case SDLK_3:
                        keys[0x03] = 0;
                        break;
                    case SDLK_q:
                        keys[0x04] = 0;
                        break;
                    case SDLK_w:
                        keys[0x05] = 0;
                        break;
                    case SDLK_e:
                        keys[0x06] = 0;
                        break;
                    case SDLK_a:
                        keys[0x07] = 0;
                        break;
                    case SDLK_s:
                        keys[0x08] = 0;
                        break;
                    case SDLK_d:
                        keys[0x09] = 0;
                        break;
                    case SDLK_z:
                        keys[0x0A] = 0;
                        break;
                    case SDLK_c:
                        keys[0x0B] = 0;
                        break;
                    case SDLK_4:
                        keys[0x0C] = 0;
                        break;
                    case SDLK_r:
                        keys[0x0D] = 0;
                        break;
                    case SDLK_f:
                        keys[0x0E] = 0;
                        break;
                    case SDLK_v:
                        keys[0x0F] = 0;
                        break;
                }
                break;
            }
        }
    }
    return quit;
}
