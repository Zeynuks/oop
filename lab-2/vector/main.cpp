#include "numbers.h"

int main()
{
	try
	{
		Numbers numbers = ReadNumbers();
		ProcessNumbers(numbers);
		PrintSortedNumbers(numbers);
	}
	catch (const std::exception& errorMessage)
	{
		std::cerr << "ERROR: " << errorMessage.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}