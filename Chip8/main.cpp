#include <cstdio>
#include "chip8.h"

using namespace std;

int main(int argc, char **argv) {

	// emulation loop
	/*while (true) {

	}*/

	chip8 game;
	game.initialize();
	game.loadGame(argv[1]);

}