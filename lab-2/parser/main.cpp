#include "parser.h"

#include <iostream>

int main()
{
	std::string rowUrl;
	UrlData url;

	std::getline(std::cin, rowUrl);
	if (!TryParseURL(rowUrl, url))
	{
		std::cout << "Error" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << rowUrl << std::endl;
	std::cout << "HOST: " << url.host << std::endl;
	std::cout << "PORT: " << url.port << std::endl;
	std::cout << "DOC: " << url.document << std::endl;

	return EXIT_SUCCESS;
}