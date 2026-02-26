#include "html_decode.h"

int main()
{
	try
	{
		const std::vector<std::string> data = ReadData(std::cin);
		PrintData(std::cout, data);
	}
	catch (const std::exception& errorMessage)
	{
		std::cerr << "ERROR: " << errorMessage.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}