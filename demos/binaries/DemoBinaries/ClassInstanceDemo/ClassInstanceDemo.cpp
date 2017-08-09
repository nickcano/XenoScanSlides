// ClassInstanceDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdint.h>
#include <iostream>
#include <chrono>
#include <thread>

enum TYPES : uint32_t
{
	TYPE_NOP = 0x90,
	TYPE_WIN = 0x1337
};


class Base
{
public:
	virtual void dostuff() = 0;
protected:
	uint32_t type;
	Base(uint32_t type) : type(type) {}
};

class Nop : public Base
{
public:
	Nop() : Base(TYPE_NOP) {}
	virtual void dostuff() { return; }
};

class Win : public Base
{
public:
	Win() : Base(TYPE_WIN) {}
	virtual void dostuff() { std::cout << "I AM A WINNER" << std::endl; }
};


void play(Base* inst)
{
	while (true)
	{
		inst->dostuff();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

int main()
{
	auto nop = new Nop();
	auto win = new Win();

	play(nop);

    return 0;
}

