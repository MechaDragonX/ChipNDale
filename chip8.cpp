#include <cstdint>
#include <fstream>
#include "chip8.hpp"

Chip8::Chip8() {
    /*
        Initialize the program counter to 0x200
        This where all instructions will begin as the ROM contents will be loaded from here
        0x000 to 0x1FF is reserved for system functions, so all ROM data is stored after that
    */
    programCounter = START_ADDRESS;
}

void Chip8::loadROM(const char* filename) {
    // Open the file isomg a bonary stream and move the file pointer to the end
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if(file.is_open()) {
        // Since the pointer is at the end, you can get the size of the file by getting the current position in the stream
        std::streampos size = file.tellg();
        // Allocate a buffer to hold the contents of the file
        char* buffer = new char[size];

        // Go back to the beginning of the file
        file.seekg(0, std::ios::beg);
        // Fill the buffer with the contents of the file
        file.read(buffer, size);
        // Close the input stream
        file.close();

        /*
            Load the contents of the rom into memory by taking each character in the buffer and setting it to each memory address, beginning with 0x200
            0x000 to 0x1FF is reserved for system functions, so all ROM data is stored after that
        */
        for(int i = 0; i < size; i++)
            memory[START_ADDRESS + i] = buffer[i];

        // Delete the buffer
        delete[] buffer;
    }
}
