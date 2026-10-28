#include "chip8.h"
#include <filesystem>
#include <fstream>
#include <ios>
#include <cstdio>

bool Chip8::loadRom(const std::string &path) {
        std::ifstream file(path,std::ios::binary);
        if (!file) {
                return false;
        }
        const auto size = std::filesystem::file_size(path);
        const std::size_t maxSize = 4096 - 0x200;
        if (size > maxSize) {
                return false;
        }
        file.read(reinterpret_cast<char*>(&memory[0x200]), static_cast<std::streamsize>(size));
        return true;
}

uint16_t Chip8::fetch() {
        const uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];
        pc +=2;
        return opcode;
}

void Chip8::cycle() {
        const uint16_t opcode = fetch();

        const uint8_t x = (opcode & 0x0F00) >> 8;
        const uint8_t y = (opcode & 0x00F0) >> 4;
        const uint8_t n = opcode & 0x000F;
        const uint8_t nn = opcode & 0x00FF;
        const uint16_t nnn = opcode & 0x0FFF;

        switch (opcode >> 12) {

                case 0x0:
                        if (opcode == 0x00E0) {   // 00E0: очистить экран
                                display.fill(false);
                        }
                        else if (opcode == 0x00EE) {
                                if (sp>0) {
                                        sp--;
                                        pc = stack[sp];
                                } else {
                                        std::printf("Stack underflow at %03X\n", pc - 2);
                                }
                        }
                        break;

                case 0x1:                     // 1NNN: прыжок
                         pc = nnn;
                         break;

                case 0x6:                     // 6XNN: V[x] = NN
                        V[x] = nn;
                        break;

                case 0x7:                     // 7XNN: V[x] += NN
                        V[x] += nn;
                        break;

                case 0x2:
                        if (sp<16) {
                                stack[sp] = pc;
                                sp++;
                                pc = nnn;
                        } else {
                                std::printf("Stack overflow at %03X\n", pc -2);
                        }
                        break;

                case 0x3:
                        if (V[x] == nn) {
                                pc += 2;
                        }
                        break;

                case 0x4:
                        if (V[x] != nn) {
                                pc += 2;
                        }
                        break;

                case 0x5:
                        if (V[x] == V[y]) {
                                pc += 2;
                        }
                        break;

                case 0x8:
                        switch(n) {
                                case 0x0:
                                        V[x] = V[y];
                                        break;

                                case 0x1:
                                        V[x] = V[x] | V[y];
                                        break;

                                case 0x2:
                                        V[x] = V[x] & V[y];
                                        break;

                                case 0x3:
                                        V[x] = V[x] ^ V[y];
                                        break;

                                case 0x4: {
                                        const int sum = V[x] + V[y];
                                        const uint8_t flag = sum > 255;
                                        V[x] = sum & 0xFF;
                                        V[0xF] = flag;
                                        break;
                                        }

                                case 0x5: {
                                        const uint8_t flag = V[x] >= V[y];   // сравнить ДО вычитания
                                        V[x] = V[x] - V[y];
                                        V[0xF] = flag;
                                        break;
                                        }

                                case 0x6:{
                                        const uint8_t flag = V[y] & 1;
                                        V[x] = V[y] >> 1;
                                        V[0xF] = flag;
                                        break;
                                }
                                case 0x7: {
                                        const uint8_t flag = V[y] >= V[x];
                                        V[x] = V[y] - V[x];
                                        V[0xF] = flag;
                                        break;
                                }
                                case 0xE: {
                                        const uint8_t flag = (V[y] >> 7) & 1;
                                        V[x] = V[y] << 1;
                                        V[0xF] = flag;
                                        break;
                                }
                                break;
                        }

                case 0x9:
                        if (V[x] != V[y]) {
                                pc += 2;
                        }
                        break;

                case 0xA:                     // ANNN: I = NNN
                        I = nnn;
                        break;

                case 0xD: {                   // DXYN: нарисовать спрайт
                        const uint8_t startX = V[x] % 64;
                        const uint8_t startY = V[y] % 32;
                        V[0xF] = 0;

                        for (int row = 0; row < n; row++) {
                                const int py = startY + row;
                                if (py >= 32) {
                                        break;
                                }
                                const uint8_t spriteByte = memory[I + row];

                                for (int col = 0; col < 8; col++) {
                                        const int px = startX + col;
                                        if (px >= 64) {
                                                break;
                                        }
                                        const bool bit = (spriteByte >> (7 - col)) & 1;
                                        if (bit) {
                                                const int index = py * 64 + px;
                                                if (display[index]) {
                                                        V[0xF] = 1;
                                                }
                                                display[index] = !display[index];
                                        }
                                }
                        }
                        break;
                }
                default:
                        std::printf("Unknown opcode: %04X\n", opcode);
                        break;

        }
}
