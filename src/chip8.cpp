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
        		break;

    		case 0x1:                     // 1NNN: прыжок
        		pc = nnn;
       			 break;

		case 0x6:                     // 6XNN: VX = NN
			V[x] = nn;
        		break;

    		case 0x7:                     // 7XNN: VX += NN
        		V[x] += nn;
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
