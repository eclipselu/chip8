#include <cstdio>
#include <iostream>
#include "chip8.h"

using namespace std;

int main(int argc, char **argv)
{

	chip8 game;
	game.initialize();
    if (argc < 2)
    {
        cout << "please specify a rom file" << endl;
		return 1;
    }
	game.loadGame(argv[1]);

	// emulation loop
	while (true)
	{
		game.emulateCycle();
	}
    return 0;
}
