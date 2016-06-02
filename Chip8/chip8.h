#pragma once
#include <string>
#include <map>

using namespace std;

const unsigned int rom_offset = 512;
const unsigned int max_memory = 4096;
const unsigned int max_width = 64;
const unsigned int max_height = 32;

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
	void op_0();
	void op_1();
	void op_2();
	void op_3();
	void op_4();
	void op_5();
	void op_6();
	void op_7();
	void op_8();
	void op_9();
    void op_a();
    void op_b();
    void op_c();
    void op_d();
	void op_e();
	void op_f();
	void convert_to_pixels(char *pixels);

	bool drawFlag;

private:
	unsigned short opcode;
	unsigned char memory[max_memory];
	unsigned char V[16];			// registers
	unsigned short I;				// index register
	unsigned short pc;				// program counter
	unsigned char delay_timer;
	unsigned char sound_timer;
	unsigned char key[16];

	unsigned short stack[16];       // stack
	unsigned short sp;              // stack pointer
	unsigned char gfx[max_width * max_height];

	opfunc_map opmap;
};

