#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include "chip8.h"

using namespace std;

chip8::chip8()
{
}


chip8::~chip8()
{
}

void chip8::initialize()
{
	memset(memory, 0, sizeof(memory));
	memset(V, 0, sizeof(V));
	memset(gfx, 0, sizeof(gfx));

	// initialize opcode map
	opmap[0] = &chip8::op_0;
	opmap[1] = &chip8::op_1;
	opmap[2] = &chip8::op_2;
	opmap[3] = &chip8::op_3;
	opmap[4] = &chip8::op_4;
	opmap[5] = &chip8::op_5;
	opmap[6] = &chip8::op_6;
	opmap[7] = &chip8::op_7;
	opmap[8] = &chip8::op_8;
	opmap[9] = &chip8::op_9;
	for (unsigned char i = 10; i <= 0x0F; i += 1)
	{
		opmap[i] = &chip8::op_null;
	}
}

void chip8::loadGame(string filename)
{
	fstream fs;
	int file_size;
	char *buffer;

	fs.open(filename, fstream::in | fstream::binary);

	if (!fs)
	{
		cerr << "no such rom file" << endl;
		exit(1);
	}

	// get file size
	fs.seekg(0, fstream::end);
	file_size = fs.tellg();
	fs.clear();
	fs.seekg(0);

	cout << "file size: " << file_size << endl;

	buffer = new char[file_size];
	fs.read(buffer, file_size);

	for (size_t i = 0; i < file_size; i++)
	{
		this->memory[i + rom_offset] = buffer[i];
	}

	this->pc = rom_offset;
	delete[] buffer;
	fs.close();
}

void chip8::fetch()
{
	if (pc + 1 >= max_memory)
        exit(0);

	opcode = memory[pc] << 8 | memory[pc + 1];
	pc += 2;
	cout << std::hex << opcode << " | ";
}

void chip8::emulateCycle()
{
	// fetch opcode
	fetch();

	// execute opcode
	cpu_opfunc func = opmap[(opcode & 0xF000) >> 12];
	(this->*func)();

	// update timers
}

void chip8::op_null()
{
	// do nothing
	cout << "this is a null op" << endl;
}

// 0nnn, 00e0, 00ee
void chip8::op_0()
{
	if (opcode == 0x00E0)
	{
		// clear display
		for (int i = 0; i < 64 * 32; i++)
			gfx[i] = 0x00;
		drawFlag = true;
	}
	else if (opcode == 0x00EE)
	{
		// return from a subroutine
		pc = stack[sp];
		sp--;
	}
	else
	{
		// jump
		// cout << "jump to a machine code routine at: " << std::hex << (opcode & 0x0FFF);
		cout << "0x0000 is ignored" << endl;
	}
}

// 1nnn
void chip8::op_1()
{
	int addr = opcode & 0xFFF;
	pc = addr;
}

// 2nnn
void chip8::op_2()
{
	int addr = opcode & 0xFFF;
	sp = sp + 1;
	stack[sp] = pc;
	pc = addr;
}

// 3xkk
void chip8::op_3()
{
	int val = opcode & 0xFF;
	int x = (opcode & 0x0F00) >> 8;

	if (V[x] == val)
		pc += 2;
}

// 4xkk
void chip8::op_4()
{
	int val = opcode & 0xFF;
	int x = (opcode & 0x0F00) >> 8;

	if (V[x] != val)
		pc += 2;
}

// 5xy0
void chip8::op_5()
{
	int x = (opcode & 0x0F00) >> 8;
	int y = (opcode & 0x00F0) >> 4;

	if (V[x] == V[y])
		pc += 2;
}

// 6xkk
void chip8::op_6()
{
	int val = opcode & 0xFF;
	int x = (opcode & 0x0F00) >> 8;
	V[x] = val;
}

// 7xkk
void chip8::op_7()
{
	int val = opcode & 0xFF;
	int x = (opcode & 0x0F00) >> 8;

	V[x] += val;
}

// 8xy0
void chip8::op_8()
{
	int x = (opcode & 0x0F00) >> 8;
	int y = (opcode & 0x00F0) >> 4;

	switch (opcode & 0xF)
	{
	case 0x0:
		// vx = vy
		V[x] = V[y];
		break;
	case 0x1:
		// vx = vx or vy
		V[x] = V[x] | V[y];
		break;
	case 0x2:
		// vx = vx and vy
		V[x] = V[x] & V[y];
	case 0x3:
		// vx = vx xor vy
		V[x] = V[x] ^ V[y];
	case 0x4:
		// vx = vx + vy
		int tmp = V[x] + V[y];
		V[0xF] = tmp > 255 ? 1 : 0;
		V[x] = tmp & 0xF;
	case 0x5:
		// vx = vx - vy
		V[0xF] = V[x] > V[y] ? 1 : 0;
		V[x] = V[x] - V[y];
	}
}

// 9xy0
void chip8::op_9()
{
	int x = (opcode & 0x0F00) >> 8;
	int y = (opcode & 0x00F0) >> 4;
	if (V[x] == V[y])
		pc += 2;
}

