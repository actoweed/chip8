#pragma once
#include <array>
#include <cstdint>
#include <string>

class Chip8{

public:
	bool loadRom(const std::string& path);
	uint16_t fetch();

private:
	std::array<uint8_t, 4096> memory{};
	std::array<uint8_t, 16> V{};
	uint16_t I = 0;
	uint16_t pc = 0x200;
	
	std::array<uint16_t, 16> stack{};
	uint8_t sp = 0;
	uint8_t delayTimer = 0;
	uint8_t soundTimer = 0;

	std::array<bool, 64 * 32> display{};
	std::array<bool, 16> keys{};

};
