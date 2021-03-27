#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>

class Chip8 {
    public:
        uint8_t registers[16];          // 16 8-bit Registers
        uint8_t memory[4096];           // Represents each of the 4096 Bytes of Memory
        uint16_t index;                 // Index Register for Soring Memory Addresses for Operations
        uint16_t programCounter;        // Adress of Next Instruction
        uint16_t stack[16];             // Execution Stack
        uint8_t stackPointer;           // Pointer Into Stack
        uint8_t delayTimer;             // Controls Timing of CPU Cycles
        uint8_t soundTimer;             // Controls Timing of Sound
        uint8_t keypad[16];             // The CHIP-8 has 16 Input Keys which are represented by 0x0 to 0xF
        uint32_t video[2048];           // Display Memory for a Display 64 pixels wide * 32 pixels tall
        uint16_t opcode;                // An Encoded Form of the Operation and Relevant Data as a Number
};

#endif
