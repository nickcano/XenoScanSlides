// StdListDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <list>
#include <stdint.h>
#include <numeric>
#include <iostream>
#include <chrono>
#include <thread>


int main()
{
	std::list<uint32_t> list;
	for (size_t i = 0; i <= 100; i++)
		list.push_back(i);

	while (true)
	{
		int result = std::accumulate(list.begin(), list.end(), 0);
		if (result != 5050)
			std::cout << "OH NO SOMEBODY MODIFIED THE LIST!" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
    return 0;
}

