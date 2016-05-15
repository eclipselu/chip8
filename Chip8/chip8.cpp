#include "chip8.h"



chip8::chip8()
{
}


chip8::~chip8()
{
}

void chip8::initialize()
{
	memset(this->memory, 0, sizeof(this->memory));
	memset(this->V, 0, sizeof(this->V));
	memset(this->gfx, 0, sizeof(this->gfx));
}

void chip8::loadGame(string filename)
{
	this->pc = 0;

	
}

void chip8::emulateCycle()
{
}
