#include <cstdio>
#include <cstdint>

const uint8_t sprite[] = {0x22, 0x77, 0x22, 0x00, 0x81, 0x42, 0x3C};

int main(){
	for (int row = 0; row < 7; row++){
		const uint8_t spriteByte = sprite[row];
		for (int col = 0; col < 8; col++){
			const bool spriteBit = (spriteByte >> (7 - col)) & 1;
			if (spriteBit) {
				std::printf("#");
      			}
			else {
				std::printf(".");
			}
		}
			std::printf("\n");
	}
}
