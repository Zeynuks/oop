#include <cstdlib>
#include <iostream>

int main(const int argc, char* argv[])
{
	try
	{

	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}