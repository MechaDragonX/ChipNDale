#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <random>
#include "chip8.hpp"

// Default Constructor
// Initializer list seeds the RNG
Chip8::Chip8() : randomGenerator(std::chrono::system_clock::now().time_since_epoch().count()) {
    /*
        Initialize the program counter to 0x200
        This where all instructions will begin as the ROM contents will be loaded from here
        0x000 to 0x1FF is reserved for system functions, so all ROM data is stored after that
    */
    programCounter = ROM_START_ADDRESS;

    // Load fonts into memory from 0x050
    for(unsigned int i = 0; i < FONTSET_LENGTH; i++)
        memory[FONTSET_START_ADDRESS + i] = fontset[i];

    randomByte = std::uniform_int_distribution<uint8_t>(0, 255u);
}

// Loads a ROM into memoery at the START_ADDRESS
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
        for(unsigned int i = 0; i < size; i++)
            memory[ROM_START_ADDRESS + i] = buffer[i];

        // Delete the buffer
        delete[] buffer;
    }
}

// CPU Instructions

// CLS - Clear Display
void Chip8::OP_00E0() {
    // Set every byte of the video buffer to zeroes
    memset(video, 0, sizeof(video));
}
// RET - Return from Subroutine
void Chip8::OP_00EE() {
    // Move the stack pointer down one so it moves past the previous instruction
    stackPointer--;
    // The make the next instruction the current program counter
    programCounter = stack[stackPointer];
}
/*
    JP <address> - Jump to Location nnn
    Sets program counter to nnn
*/
void Chip8::OP_1nnn() {
    uint16_t address = opcode & 0x0FFFu;
    programCounter = address;
}
// CALL <address> - Call Subroutine at nnn
void Chip8::OP_2nnn() {
    uint16_t address = opcode & 0x0FFFu;

    stack[stackPointer] = programCounter;
    stackPointer++;
    programCounter = address;
}
// SE Vx, <byte> - Skip Next Instruction if Vx = kk
void Chip8::OP_3xkk() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    if(registers[vX] == byte)
        programCounter += 2;
}
// SNE Vx, <byte> - Skip Next instruction if Vx != kk
void Chip8::OP_4xkk() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    if(registers[vX] != byte)
        programCounter += 2;
}
// SE Vx, Vy - Skip Next Instruction if Vx = Vy
void Chip8::OP_5xy0() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t vY = (opcode & 0x00F0u) >> 4u;
    if(registers[vX] == registers[vY])
        programCounter += 2;
}
// LD Vx, <byte> - Set Vx = kk
void Chip8::OP_6xkk() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    registers[vX] = byte;
}
// ADD Vx, <byte> - Vx += kk
void Chip8::OP_7xkk() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    registers[vX] += byte;
}
// LD Vx, Vy - Set Vx = Vy
void Chip8::OP_8xy0() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t vY = (opcode & 0x00F0u) >> 4u;
    registers[vX] = registers[vY];
}
// OR Vx, Vy - Set Vx |= Vy
void Chip8::OP_8xy1() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t vY = (opcode & 0x00F0u) >> 4u;
    registers[vX] |= registers[vY];
}
// AND Vx, Vy - Set Vx &= Vy
void Chip8::OP_8xy2() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t vY = (opcode & 0x00F0u) >> 4u;
    registers[vX] &= registers[vY];
}
// XOR Vx, Vy - Set Vx ^= Vy
void Chip8::OP_8xy3() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t vY = (opcode & 0x00F0u) >> 4u;
    registers[vX] ^= registers[vY];
}
/*
    ADD Vx, Vy - Set Vx += Vy, Set VF = Carry
    If sum is greater than 8 bits, VF is set to 1, otherwise 0
    Only the lowest 8 bits of sum are kept and stored in Vx
*/
void Chip8::OP_8xy4() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t vY = (opcode & 0x00F0u) >> 4u;

    uint16_t sum = registers[vX] + registers[vY];
    if(sum > 255u)
        registers[0x0F] = 1;
    else
        registers[0x0F] = 0;

    registers[vX] = sum & 0xFFu;
}
/*
    SUB Vx, Vy - Set Vx -= Vy, Set VF = Not borrow
    If Vx > Vy, then VF is set to 1, otherwise 0
    Then Vy is subtracted from Vx, and result's stored in Vx
*/
void Chip8::OP_8xy5() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t vY = (opcode & 0x00F0u) >> 4u;

    if(registers[vX] > registers[vY])
        registers[0x0F] = 1;
    else
        registers[0x0F] = 0;

    registers[vX] -= registers[vY];
}
/*
    SHR Vx - Set Vx >>= 1
    If least-significant bit of Vx is 1, the VF is set to 1, otherwise 0
    Then, vX is divided by 2 (aka right shift)
*/
void Chip8::OP_8xy6() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;

    // Save least-significant bit in VF
    registers[0x0F] = registers[vX] & 0x01u;

    registers[vX] >>= 1;
}
/*
    SUBN Vx, Vy - Set Vx = Vy - Vx, Set VF - Not Borrow
    If Vx > Vy, then VF is set to 1, otherwise 0
    Then Vx is subtracted from Vy, and result's stored in Vx
*/
void Chip8::OP_8xy7() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t vY = (opcode & 0x00F0u) >> 4u;

    if(registers[vX] > registers[vY])
        registers[0x0F] = 1;
    else
        registers[0x0F] = 0;

    registers[vX] = registers[vY] - registers[vX];
}
/*
    SHL Vx (), Vy) - Set Vx <<= 1
    If most-significant bit of Vx is 1, the VF is set to 1, otherwise 0
    Then, vX is multiplied by 2 (aka left shift)
*/
void Chip8::OP_8xyE() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;

    // Save most-signifacnt bit in VF
    registers[0x0F] = (registers[vX] & 0x80u) >> 7u;

    registers[vX] <<= 1;
}
// SNE Vx, Vy - Skip Next Instruction if Vx != Vy
void Chip8::OP_9xy0() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t vY = (opcode & 0x00F0u) >> 4u;

    if(registers[vX] != registers[vY])
        programCounter += 2;
}
// LD I, <address> - Set I = nnn
void Chip8::OP_Annn() {
    uint16_t address = opcode & 0x0FFFu;
    index = address;
}
// JP V0, <address> - Jump to Location nnn + V0
void Chip8::OP_Bnnn() {
    uint16_t address = opcode & 0x0FFFu;
    programCounter = registers[0] + address;
}
// RND Vx, <byte> - Set Vx = <random byte> & kk
void Chip8::OP_Cxkk() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    registers[vX] = randomByte(randomGenerator) & byte;
}
// DRW Vx, Vy, <nibble> - Display n-byte sprite starting at memory Location I at (Vx, Vy), Set VF = Collision
void Chip8::OP_Dxyn() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t vY = (opcode & 0x00F0u) >> 4u;
    uint8_t height = opcode & 0x000Fu;

    // WRap if going beyond screen boundaries
    uint8_t xPosition = registers[vX] % VIDEO_WIDTH;
    uint8_t yPosition = registers[vY] % VIDEO_HEIGHT;

    // Set VF to 0 as default
    registers[0x0F] = 0;
    // Look through every position in the sprite
    // i is row number
    for(unsigned int i = 0; i < height; i++) {
        uint8_t spriteByte = memory[index + i];
        // j is column number
        for(unsigned int j = 0; j < SPRITE_WIDTH; j++) {
            uint8_t spritePixel = spriteByte & (0x80u >> j);
            uint32_t* screenPixel = &video[(yPosition + i) * VIDEO_WIDTH + (xPosition + j)];
            // If the screen pixel is on
            if(spritePixel) {
                // Check if there is a collision, i.e. if the current pixel is also on
                if(*screenPixel == 0xFFFFFFFF) {
                    // if so, set VF to 1
                    registers[0x0F] = 1;
                }
                // Ostensibly XOR with sprite pixel (cannot directly as spritePixel is 0 or 1)
                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}
// SKP Vx - Skip next instruction if key of value Vx is pressed
void Chip8::OP_Ex9E() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[vX];
    if(keypad[key]) {
        programCounter += 2;
    }
}
// SKNP Vx - Skip next instruction if key of value Vx is not pressed
void Chip8::OP_ExA1() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[vX];
    if(!keypad[key]) {
        programCounter += 2;
    }
}
// LD Vx, DT - Set Vx = <Delay Time Value>
void Chip8::OP_Fx07() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    registers[vX] = delayTimer;
}
// LD Vx, L - Wait for key press, store value of key in Vx
void Chip8::OP_Fx0A() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    // Check every single key if pressed, if so store value to Vx
    if(keypad[0]) {
        registers[vX] = 0;
    } else if(keypad[1]) {
        registers[vX] = 1;
    } else if(keypad[2]) {
        registers[vX] = 2;
    } else if(keypad[3]) {
        registers[vX] = 3;
    } else if(keypad[4]) {
        registers[vX] = 4;
    } else if(keypad[5]) {
        registers[vX] = 5;
    } else if(keypad[6]) {
        registers[vX] = 6;
    } else if(keypad[7]) {
        registers[vX] = 7;
    } else if(keypad[8]) {
        registers[vX] = 8;
    } else if(keypad[9]) {
        registers[vX] = 9;
    } else if(keypad[10]) {
        registers[vX] = 10;
    } else if(keypad[11]) {
        registers[vX] = 11;
    } else if(keypad[12]) {
        registers[vX] = 12;
    } else if(keypad[13]) {
        registers[vX] = 13;
    } else if(keypad[14]) {
        registers[vX] = 14;
    } else if(keypad[15]) {
        registers[vX] = 15;
    // Otherwise, decrement program counter by 2 as a way of "waiting"
    } else {
        programCounter -= 2;
    }

    // Find a better way to do the above. A loop could work, but the logic of only decrementing the program once on exit is a bit whack
}
// LD DT, Vx - Set Delay Timer to Vx
void Chip8::OP_Fx15() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    delayTimer = registers[vX];
}
// LD ST, Vx - Set Sound Timer to Vx
void Chip8::OP_Fx18() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    soundTimer = registers[vX];
}
// ADD I, Vx - Set I += Vx
void Chip8::OP_Fx1E() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    index += registers[vX];
}
// LD F, Vx - Set I to Locaiton of Sprite for Digit Vx
void Chip8::OP_Fx29() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t digit = registers[vX];
    // Offset index based on the character
    index = FONTSET_START_ADDRESS + CHARACTER_LENGTH * digit;
}
/*
    LD B, Vx - Store binary coded decimal representation of Vx in memory location I, I+1, I+2
    Takes decimal value of Vx and places hundreds digit in memory at location I, tens digit at I+1, and ones digit at I+2
*/
void Chip8::OP_Fx33() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    uint8_t value = registers[vX];

    // Store ones digit
    memory[index + 2] = value % 10;
    // Shave off a digit
    value /= 10;

    // Store tens digit
    memory[index + 1] = value % 10;
    // Shave off a digit
    value /= 10;

    // Store hundred digit
    memory[index] = value % 10;
}
// LD [I], Vx - Store registers V0 to Vx in memeory strating at memory location I
void Chip8::OP_Fx55() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    for(uint8_t i = 0; i <= vX; i++) {
        memory[index + i] = registers[i];
    }
}
// LD Vx, [I] - Read registers V0 to Vx from memory strating at memory location I
void Chip8::OP_Fx65() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;
    for(uint8_t i = 0; i <= vX; i++) {
        registers[i] = memory[index + i];
    }
}
