#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <chrono>
#include <cstdint>
#include <random>

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

        std::default_random_engine randomGenerator;
        std::uniform_int_distribution<uint8_t> randomByte;

        Chip8();                                            // Default Constructor

        void loadROM(const char* filename);                 // Loads a ROM into memoery at the START_ADDRESS

        // CPU Instructions

        void OP_00E0();                                     // CLS - Clear Display
        void OP_00EE();                                     // RET - Return from Subroutine
        void OP_1nnn();                                     // JP <address> - Jump to Location nnn
        void OP_2nnn();                                     // CALL <address> - Call Subroutine at nnn
        void OP_3xkk();                                     // SE Vx, <byte> - Skip Next Instruction if Vx = kk
        void OP_4xkk();                                     // SNE Vx, <byte> - Skip Next instruction if Vx != kk
        void OP_5xy0();                                     // SE Vx, Vy - Skip Next Instruction if Vx = Vy
        void OP_6xkk();                                     // LD Vx, <byte> - Set Vx = kk
        void OP_7xkk();                                     // ADD Vx, <byte> - Vx += kk
        void OP_8xy0();                                     // LD Vx, Vy - Set Vx = Vy
        void OP_8xy1();                                     // OR Vx, Vy - Set Vx |= Vy
        void OP_8xy2();                                     // AND Vx, Vy - Set Vx &= Vy
        void OP_8xy3();                                     // XOR Vx, Vy - Set Vx ^= Vy
        void OP_8xy4();                                     // ADD Vx, Vy - Set Vx += Vy, Set VF = Carry
        void OP_8xy5();                                     // SUB Vx, Vy - Set Vx -= Vy, Set VF = Not borrow
        void OP_8xy6();                                     // SHR Vx - Set Vx >>= 1
        void OP_8xy7();                                     // SUBN Vx, Vy - Set Vx = Vy - Vx, Set VF - Not Borrow
        void OP_8xyE();                                     // SHL Vx (), Vy) - Set Vx <<= 1
        void OP_9xy0();                                     // SNE Vx, Vy - Skip Next Instruction if Vx != Vy
        void OP_Annn();                                     // LD I, <address> - Set I = nnn
        void OP_Bnnn();                                     // JP V0, <address> - Jump to Location nnn + V0
        void OP_Cxkk();                                     // RND Vx, <byte> - Set Vx = <random byte> & kk
        void OP_Dxyn();                                     // DRW Vx, Vy, <nibble> - Display n-byte sprite starting at memory Location I at (Vx, Vy), Set VF = Collision
        void OP_Ex9E();                                     // SKP Vx - Skip next instruction if key of Value Vx is pressed
        void OP_ExA1();                                     // SKNP Vx - Skip next instruction if key of Value Vx is not pressed
        void OP_Fx07();                                     // LD Vx, DT - Set Vx = <Delay Time Value>
        void OP_Fx0A();                                     // LD Vx, L - Wait for key press, store value of key in Vx
        void OP_Fx15();                                     // LD DT, Vx - Set Delay Timer to Vx
        void OP_Fx18();                                     // LD ST, Vx - Set Sound Timer to Vx
        void OP_Fx1E();                                     // ADD I, Vx - Set I += Vx
        void OP_Fx29();                                     // LD F, Vx - Set I to Locaiton of Sprite for Digit Vx
        void OP_Fx33();                                     // LD B, Vx - Store binary coded decimal representation of Vx in memory location I, I+1, I+2
        void OP_Fx55();                                     // LD [I], Vx - Store registers V0 to Vx in memeory strating at memory location I
        void OP_Fx65();                                     // LD Vx, [I] - Read registers V0 to Vx from memory strating at memory location I
};

#endif
