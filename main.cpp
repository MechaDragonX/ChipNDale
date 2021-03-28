#include <chrono>
#include <iostream>
#include "chip8.hpp"
#include "renderer.hpp"

const int VIDEO_WIDTH = 64;     // Width of Video Display
const int VIDEO_HEIGHT = 32;    // Height of Video Display

int main(int argc, char** argv) {
    if(argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <Display Scale Factor> <Cycle Speed in ms> <Path to ROM>\n";
        return 1;
    }

    // Parse arguments
    int displayScale = std::stoi(argv[1]);
    int cycleSpeed = std::stoi(argv[2]);
    const char* romPath = argv[3];

    // Create the window using the rendering
    Renderer renderer("Chip 'n Dale - CHIP-8 Emulator", VIDEO_WIDTH * displayScale, VIDEO_HEIGHT * displayScale, VIDEO_WIDTH, VIDEO_HEIGHT);

    // Create the CHIP-8 and load the rOM into memory.
    Chip8 chip8;
    chip8.loadROM(romPath);

    // Number of bytes in a row of pixel data; used for SDL Window updating
    int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;
    // Start time used for cycle calculations
    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    // Should the program end or not?
    bool quit = false;
    while(!quit) {
        // Check to quite by checking if the "ESC" key has been pressed
        quit = renderer.processInput(chip8.keypad);
        
        // Get the current time
        auto currentTime = std::chrono::high_resolution_clock::now();
        // Get the change in time by subtracting the lastCycleTime from the currentTime
        float deltaTime = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();
        // If the time passed is greater than the clock speed
        if(deltaTime > cycleSpeed) {
            // Reset variable
            lastCycleTime = currentTime;
            // Execute a cycle
            chip8.cycle();
            // Update the window with the new information
            renderer.update(chip8.video, videoPitch);
        }
    }

    // Proper exit
    return 0;
}
