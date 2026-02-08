#include "replace_core.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>

using namespace std::literals;

enum class AppMode
{
	File,
	Stdin,
	Help,
	Invalid
};

AppMode DefineAppMode(const int argc, char* argv[])
{
	switch (argc)
	{
	case 5:
		return AppMode::File;
	case 1:
		return AppMode::Stdin;
	case 2:
		if (std::string_view(argv[1]) == "-h"sv || std::string_view(argv[1]) == "--help"sv)
		{
			return AppMode::Help;
		}
		break;
	default:
		break;
	}
	return AppMode::Invalid;
}

void ProcessFile(const std::string& inputFileName,
	const std::string& outputFileName,
	const std::string& search,
	const std::string& replace)
{
	std::ifstream input(inputFileName);
	if (!input.is_open())
	{
		throw std::invalid_argument("Failed to open input file: " + inputFileName);
	}

	std::ofstream output(outputFileName);
	if (!output.is_open())
	{
		throw std::invalid_argument("Failed to open output file: " + outputFileName);
	}

	std::string line;
	while (std::getline(input, line))
	{
		output << ReplaceString(line, search, replace) << std::endl;
	}

	input.close();
	output.close();
}

void ProcessStdin()
{
	std::string search, replace;
	std::string line;

	std::getline(std::cin, search);
	std::getline(std::cin, replace);

	if (!std::getline(std::cin, line) || line.empty())
	{
		throw std::runtime_error("Replacement string is empty");
	}

	std::cout << ReplaceString(line, search, replace) << std::endl;
}

void ShowHelpMessage()
{
	std::cout
		<< "Usage:" << std::endl
		<< "  replace <inputFile> <outputFile> <searchString> <replacementString>" << std::endl
		<< "  replace               # interactive stdin mode" << std::endl
		<< "  replace -h | --help   # show this help message" << std::endl;
}

int main(const int argc, char* argv[])
{
	const AppMode mode = DefineAppMode(argc, argv);

	try
	{
		switch (mode)
		{
		case AppMode::File:
			ProcessFile(argv[1], argv[2], argv[3], argv[4]);
			break;
		case AppMode::Stdin:
			ProcessStdin();
			break;
		case AppMode::Help:
			ShowHelpMessage();
			break;
		case AppMode::Invalid:
			throw std::runtime_error("Invalid arguments. Use -h | --help for usage info.");
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
