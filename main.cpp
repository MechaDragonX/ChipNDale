#include <chrono>
#include <iostream>
#include "chip8.hpp"
#include "renderer.hpp"

int main(int argc, char** argv) {
    if(argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
        return 1;
    }
}
