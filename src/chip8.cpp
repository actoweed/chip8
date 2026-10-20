#include "chip8.h"
#include <filesystem>
#include <fstream>
#include <ios>

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
