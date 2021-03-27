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
    for(int i = 0; i < 80; i++)
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
        for(int i = 0; i < size; i++)
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
    // Move the stack pointer down one so it moves paster the previous instruction
    stackPointer--;
    // The make the next instruciton the current program counter
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
    If least-significant bit of Vx is 1, the VF is set to 1, otherwise 0
    Then, vX is multiplied by 2 (aka left shift)
*/
void Chip8::OP_8xyE() {
    uint8_t vX = (opcode & 0x0F00u) >> 8u;

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
// JP V0, <address> - Jump to Location nnn + V0
// RND Vx, <byte> - Set Vx = <random byte> & kk
// DRW Vx, Vy, <nibble> - Display n-byte sprite starting at memory Location I at (Vx, Vy), Set VF = Collision
// SKP Vx - Skip next instruction if key of Value Vx is pressed
// SKNP Vx - Skip next instruction if key of Value Vx is not pressed
// LD Vx, DT - Set Vx = <Delay Time Value>
// LD Vx, L - Wait for key press, store value of key in Vx
// LD DT, Vx - Set Delay Timer to Vx
// LD ST, Vx - Set Sound Timer to Vx
// ADD I, Vx - Set I += Vx
// LD F, Vx - Set I to Locaiton of Sprite for Digit Vx
// LD B, Vx - Store binary coded decimal representation of Vx in memory location I, I+1, I+2
// LD [I], Vx - Store registers V0 to Vx in memeory strating at memory location I
// LD Vx, [I] - Read registers V0 to Vx from memory strating at memory location I
