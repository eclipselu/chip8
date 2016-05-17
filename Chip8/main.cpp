#include <cstdio>
#include "chip8.h"

using namespace std;

int main(int argc, char **argv)
{

	chip8 game;
	game.initialize();
	game.loadGame(argv[1]);

	// emulation loop
	while (true) 
	{
		game.emulateCycle();
	}
}