// StdMapDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <thread>

int main()
{
	std::map<uint32_t, std::string> numbers;

	numbers[1] = "one  ";
	numbers[2] = "two  ";
	numbers[3] = "three";
	numbers[4] = "four ";
	numbers[5] = "five ";

	while (true)
	{
		system("cls");
		for (auto entry = numbers.begin(); entry != numbers.end(); entry++)
			std::cout << entry->second << "  " << entry->first << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}


    return 0;
}

