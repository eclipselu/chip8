#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <cstdlib>
#include <memory>
#include "chip8.h"

using namespace std;

unsigned char chip8_fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


chip8::chip8()
{
}


chip8::~chip8()
{
}

void chip8::initialize()
{
    srand(time(0));

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
	opmap[0xa] = &chip8::op_a;
	opmap[0xb] = &chip8::op_b;
	opmap[0xc] = &chip8::op_c;
	opmap[0xd] = &chip8::op_d;
	opmap[0xe] = &chip8::op_e;
	opmap[0xf] = &chip8::op_f;
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

	sp = 0;
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
}

void chip8::emulateCycle()
{
	// fetch opcode
	fetch();

	// execute opcode
	cpu_opfunc func = opmap[(opcode & 0xF000) >> 12];
	(this->*func)();

	// update timers
	if (delay_timer > 0)
		delay_timer--;

	if (sound_timer > 0)
	{
		if (sound_timer == 1)
			cout << "beep" << endl;
		sound_timer--;
	}
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
	stack[sp] = pc;
	sp = sp + 1;
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
		break;
	case 0x3:
		// vx = vx xor vy
		V[x] = V[x] ^ V[y];
		break;
	case 0x4:
		// vx = vx + vy
		V[0xF] = V[x] + V[y] > 255 ? 1 : 0;
		V[x] = (V[x] + V[y]) & 0xF;
		break;
	case 0x5:
		// vx = vx - vy
		V[0xF] = V[x] > V[y] ? 1 : 0;
		V[x] = V[x] - V[y];
		break;
	case 0x6:
		// vx = vx shr 1
		V[0xF] = V[x] & 0x1;
		V[x] = V[x] >> 1;
		break;
	case 0x7:
		V[0xF] = V[x] < V[y] ? 1 : 0;
		V[x] = V[x] - V[y];
		break;
	case 0xE:
		V[0xF] = V[x] & 0x1;
		V[x] = V[x] << 1;
		break;
	default:
		break;
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

// annn
void chip8::op_a()
{
	int val = opcode & 0xFFF;
	I = val;
}

// bnnn
void chip8::op_b()
{
    int addr = opcode & 0xFFF;
    pc = V[0] + addr;
}

// cxkk
void chip8::op_c()
{
    int x = (opcode & 0x0F00) >> 8;
    char r = rand() % 256;
    V[x] = V[x] & r;
}

// dxyn
void chip8::op_d()
{
    // draw
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;
    int n = opcode & 0xF;
	unsigned short pixel;

	V[0xF] = 0;
	for (int y_ = 0; y_ < n; y_++)
	{
		pixel = memory[I + y_];
		for (int x_ = 0; x_ < 8; x_++)
		{
			// if pixel[x_] == 1
			if (pixel & (0x80 >> x_) != 0)
			{
				int pos = (x + x_ + (y + y_) * max_width);
				if (gfx[pos] == 1)
					V[0xF] = 1;
				gfx[pos] ^= 1;
			}
		}
	}
	drawFlag = true;

}

// keyboard related
void chip8::op_e()
{
}

void chip8::op_f()
{
	int x = opcode & 0x0F00 >> 8;
	int n = opcode & 0xFF;

	switch (n)
	{
	case 0x07:
		V[x] = delay_timer;
		break;
	case 0x0A:
		// todo
		break;
	case 0x15:
		delay_timer = V[x];
		break;
	case 0x18:
		sound_timer = V[x];
		break;
	case 0x1E:
		I += V[x];
		break;
	case 0x29:
		I = chip8_fontset[5 * V[x]];
		break;
	case 0x33:
		memory[I] = V[x] / 100;
		memory[I + 1] = V[x] % 100 / 10;
		memory[I + 2] = V[x] % 10;
		break;
	case 0x55:
		memcpy(memory + I, V, sizeof(V));
		break;
	case 0x65:
		memcpy(V, memory + I, sizeof(V));
		break;
	default:
		break;
	}
}

void chip8::convert_to_pixels(char * pixels)
{
	for (size_t i = 0; i < max_width * max_height; i++)
		pixels[i] = gfx[i] * 255;
}
