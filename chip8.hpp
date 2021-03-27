#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>

class Chip8 {
    private:
        const unsigned int START_ADDRESS = 0x200;   // Address which the contents of ROMS are loaded into memory
    public:
        uint8_t registers[16];                      // 16 8-bit Registers
        uint8_t memory[4096];                       // Represents each of the 4096 bytes of memory
        uint16_t index;                             // Index register for storing memory addresses for aperations
        uint16_t programCounter;                    // Adress of Next Instruction
        uint16_t stack[16];                         // Execution Stack
        uint8_t stackPointer;                       // Pointer Into Stack
        uint8_t delayTimer;                         // Controls Timing of CPU Cycles
        uint8_t soundTimer;                         // Controls Timing of Sound
        uint8_t keypad[16];                         // The CHIP-8 has 16 Input Keys which are represented by 0x0 to 0xF
        uint32_t video[2048];                       // Display memory for a display 64 pixels wide * 32 pixels tall
        uint16_t opcode;                            // An encoded form of the operation and relevant data as a number

        Chip8();                                    // Default Constructors; Intializes Program Counter

        void loadROM(const char* filename);         // Loads a ROM into memoery at the START_ADDRESS
};

#endif
