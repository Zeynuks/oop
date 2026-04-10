#include "EconomyEmulator.hpp"

#include <exception>
#include <iostream>

int main()
{

	try
	{
		int iterationCount;
		EconomyEmulator simulator;

		simulator.InitEmulator();

		std::cout << "Input iteration count: ";
		std::cin >> iterationCount;

		simulator.Emulate(iterationCount);
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}