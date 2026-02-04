#include "MatrixLib/Matrix.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

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
	case 1:
		return AppMode::Stdin;
	case 2:
		if (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0)
		{
			return AppMode::Help;
		}
		return AppMode::File;
	default:
		break;
	}
	return AppMode::Invalid;
}

void ProcessStream(std::istream& in)
{
	MatrixLib::Matrix<float> m(3, 3);
	in >> m;

	std::cout << m.inverse() << std::endl;
}

void ProcessFile(const char* filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file");
	}

	ProcessStream(file);
}

void ShowHelpMessage()
{
	std::cout << "Usage: invert [option] [file]\n"
			  << "Options:\n"
			  << "  -h            Show this help message\n"
			  << "If no file is specified, input is expected from stdin.\n";
}

int main(const int argc, char* argv[])
{
	const AppMode mode = DefineAppMode(argc, argv);

	try
	{
		switch (mode)
		{
		case AppMode::File:
			ProcessFile(argv[1]);
			break;
		case AppMode::Stdin:
			ProcessStream(std::cin);
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
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
