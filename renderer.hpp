#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

class Renderer {
    private:
        SDL_Window* window_;                                                // SDL Object Representing the Window
        SDL_Renderer* renderer_;                                            // SDL Object Represnting the 2D Rendering Context for a Window
        SDL_Texture* texture_;                                              // SDL Object Representing the Texture the Window Uses
    public:
        Renderer(const char* title, int windowWidth, int windowHeight,      // Creates Display Window
            int textureWidth, int textureHeight);
        ~Renderer();                                                        // Destroy each field object and stops displaying graphics

        void update(const void* buffer, int pitch);                         // Update the Display with New Information
        bool processInput(uint8_t* keys);                                   // Decide Action for each Key
};

#endif
