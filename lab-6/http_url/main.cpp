#include "HttpUrl.hpp"
#include <iostream>
#include <string>

int main()
{
	std::string urlString;
	while (std::getline(std::cin, urlString))
	{
		if (urlString.empty())
		{
			continue;
		}

		try
		{
			HttpUrl url(urlString);
			std::cout << url.GetURL() << std::endl;
		}
		catch (const UrlParsingError& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	return 0;
}