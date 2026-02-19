#include "ExpandTemplate.hpp"
#include "Parsers.h"
#include "TemplateStorage.h"

#include <fstream>
#include <iostream>
#include <string_view>

using namespace std::string_view_literals;

enum class AppMode
{
	File,
	Stdin,
	Help,
	Invalid
};

AppMode DefineAppMode(const int argc, char* argv[])
{
	if (argc == 1)
	{
		return AppMode::Stdin;
	}

	if (argc == 2)
	{
		if (const std::string_view arg = argv[1]; arg == "-h"sv || arg == "--help"sv)
		{
			return AppMode::Help;
		}
		return AppMode::Invalid;
	}

	if ((argc - 3) % 2 == 0)
	{
		return AppMode::File;
	}

	return AppMode::Invalid;
}

template <typename T>
concept FileStream = std::same_as<T, std::ifstream> || std::same_as<T, std::ofstream>;

template <FileStream T>
void TryOpenFile(const std::string& filename, T& file)
{
	file.open(filename);
	if (!file.is_open())
	{
		throw std::runtime_error("Cannot open file: " + filename);
	}
}

void ExpandTemplateStream(std::istream& input, std::ostream& output, const TemplateStorage& storage)
{
	const std::string tpl{ std::istreambuf_iterator(input), std::istreambuf_iterator<char>() };

	const auto& templates = storage.GetAllTemplates();

	output << ExpandTemplate(tpl, templates);
}

void ApplyTemplate(int argc, char* argv[], TemplateStorage& storage)
{
	ParseTemplates(argc, argv, storage);

	std::ifstream input;
	std::ofstream output;
	TryOpenFile(argv[1], input);
	TryOpenFile(argv[2], output);

	ExpandTemplateStream(input, output, storage);
}

void ApplyTemplate(TemplateStorage& storage)
{
	ParseTemplates(storage);

	ExpandTemplateStream(std::cin, std::cout, storage);
}

inline void ShowHelpMessage()
{
	std::cout
		<< "Usage:\n"
		<< "  expand_template <inputFile> <outputFile> <key> <value> [<key> <value> ...]\n"
		<< "  expand_template               # interactive stdin mode\n"
		<< "  expand_template -h | --help  # show this help message\n";
}

void ExecuteApplication(const AppMode mode, const int argc, char* argv[])
{
	TemplateStorage storage;

	switch (mode)
	{
	case AppMode::File:
		ApplyTemplate(argc, argv, storage);
		break;
	case AppMode::Stdin:
		ApplyTemplate(storage);
		break;
	case AppMode::Help:
		ShowHelpMessage();
		break;
	default:
		throw std::runtime_error("Invalid arguments. Use -h or --help.");
	}
}

int main(const int argc, char* argv[])
{
	const AppMode mode = DefineAppMode(argc, argv);

	try
	{
		ExecuteApplication(mode, argc, argv);
	}
	catch (const std::exception& e)
	{
		std::cerr << std::endl
				  << "ERROR: "
				  << e.what()
				  << std::endl;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}