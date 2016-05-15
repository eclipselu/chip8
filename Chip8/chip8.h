#pragma once
#include <string>

using namespace std;

class chip8
{
public:
	chip8();
	~chip8();
	void initialize();
	void loadGame(string filename);
	void emulateCycle();

private:
	unsigned short opcode;
	unsigned char memory[4096];
	unsigned char V[16];			// registers
	unsigned short I;				// index register
	unsigned short pc;				// program counter
	unsigned char gfx[64 * 32];
	unsigned char delay_timer;
	unsigned char sound_timer;
	bool drawFlag;
	unsigned char key[16];

	unsigned short stack[16];       // stack
	unsigned short sp;              // stack pointer

};

