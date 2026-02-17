#include "ExpressionParser.h"
#include "Stack.hpp"

#include <iostream>

int main()
{
	Stack<Expression> stack;

	try
	{
		std::string line;
		while (std::getline(std::cin, line))
		{
			if (!line.empty())
			{
				const long long result = ParseExpression(line);
				std::cout << result << std::endl;
			}
		}
	}
	catch (const std::exception& exception)
	{
		std::cerr << "ERROR: " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
