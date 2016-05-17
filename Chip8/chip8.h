#pragma once
#include <string>
#include <map>

using namespace std;

const unsigned int rom_offset = 512;
const unsigned int max_memory = 4096;

class chip8
{
typedef void (chip8::*cpu_opfunc)();  // function pointer to class chip8's member function
typedef map<unsigned char, cpu_opfunc> opfunc_map;  // int -> cpu_opfunc

public:
	chip8();
	~chip8();
	void initialize();
	void loadGame(string filename);
	void fetch();
	void emulateCycle();
	void op_null();

private:
	unsigned short opcode;
	unsigned char memory[max_memory];
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

	opfunc_map opmap;
};

