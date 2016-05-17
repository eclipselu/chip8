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
	memset(this->memory, 0, sizeof(this->memory));
	memset(this->V, 0, sizeof(this->V));
	memset(this->gfx, 0, sizeof(this->gfx));

	// initialize opcode map
	for (unsigned char i = 0; i < 0x0F; i += 1)
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
	this->opcode = memory[this->pc] << 8 | memory[this->pc + 1];
	this->pc += 2;
}

void chip8::emulateCycle()
{
	// fetch opcode
	fetch();

	// execute opcode
	cpu_opfunc func = this->opmap[(this->opcode & 0xF000) >> 12];
	(this->*func)();

	// update timers
}

void chip8::op_null()
{
	// do nothing
	cout << "this is a null op" << endl;
}
