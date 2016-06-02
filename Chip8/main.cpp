#include <cstdio>
#include <iostream>
#include <SDL.h>
#include "chip8.h"


using namespace std;

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow("SDL2 Pixel Drawing",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64, 32, 0);

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture * texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 64, 32);

	chip8 game;
	game.initialize();
    if (argc < 2)
    {
        cout << "please specify a rom file" << endl;
		return 1;
    }
	game.loadGame(argv[1]);
	char *pixels = new char[64 * 32];
	// emulation loop
	while (true)
	{
		game.emulateCycle();

		if (game.drawFlag)
		{
			game.convert_to_pixels(pixels);
			SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(char));
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	delete[] pixels;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);
	SDL_Quit();

    return 0;
}
