#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>

class Chip8 {
    private:
        const unsigned int ROM_START_ADDRESS = 0x200;       // Address at which the contents of ROMs are loaded into memory
        const unsigned int FONTSET_START_ADDRESS = 0x050;   // Address at which the fontset is loaded into memory
    public:
        // CPU State Information
        uint8_t registers[16];                              // 16 8-bit Registers
        uint8_t memory[4096];                               // Represents each of the 4096 bytes of memory
        uint16_t index;                                     // Index register for storing memory addresses for aperations
        uint16_t programCounter;                            // Adress of Next Instruction
        uint16_t opcode;                                    // An encoded form of the operation and relevant data as a number
        uint16_t stack[16];                                 // Execution Stack
        uint8_t stackPointer;                               // Pointer Into Stack
        uint8_t delayTimer;                                 // Controls Timing of CPU Cycles
        
        // I/O State Information
        uint8_t soundTimer;                                 // Controls Timing of Sound
        uint8_t keypad[16];                                 // The CHIP-8 has 16 Input Keys which are represented by 0x0 to 0xF
        uint32_t video[2048];                               // Display memory for a display 64 pixels wide * 32 pixels tall

        uint8_t fontset[80] = {                             // 80 bytes that represents all the chracters the screen can display
            0xF0, 0x90, 0x90, 0x90, 0xF0,                       // 0
            0x20, 0x60, 0x20, 0x20, 0x70,                       // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0,                       // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0,                       // 3
            0x90, 0x90, 0xF0, 0x10, 0x10,                       // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0,                       // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0,                       // 6
            0xF0, 0x10, 0x20, 0x40, 0x40,                       // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0,                       // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0,                       // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90,                       // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0,                       // B
            0xF0, 0x80, 0x80, 0x80, 0xF0,                       // C
            0xE0, 0x90, 0x90, 0x90, 0xE0,                       // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0,                       // E
            0xF0, 0x80, 0xF0, 0x80, 0x80                        // F
        };

        Chip8();                                            // Default Constructor

        void loadROM(const char* filename);                 // Loads a ROM into memoery at the START_ADDRESS
};

#endif
