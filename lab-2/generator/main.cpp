#include "prime_generator.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

using namespace std::literals;

constexpr int MAX_VALUE = 100'000'000;
constexpr int MIN_VALUE = 0;

enum class AppMode
{
	Argument,
	Stdin,
	Help,
	Invalid
};

AppMode DefineAppMode(const int argc, char* argv[])
{
	switch (argc)
	{
	case 2:
		if (std::string_view(argv[1]) == "-h"sv || std::string_view(argv[1]) == "--help"sv)
		{
			return AppMode::Help;
		}
		return AppMode::Argument;

	case 1:
		return AppMode::Stdin;

	default:
		return AppMode::Invalid;
	}
}

int ParseUpperBound(const std::string& value)
{
	const int upperBound = std::stoi(value);

	if (upperBound < MIN_VALUE || upperBound > MAX_VALUE)
	{
		throw std::out_of_range("upperBound must be between 0 and 100000000");
	}

	return upperBound;
}

void PrintPrimes(const int upperBound)
{
	const auto primes = GeneratePrimeNumbersSet(upperBound);

	for (const int p : primes)
	{
		std::cout << p << " ";
	}

	std::cout << std::endl
			  << "Count = " << primes.size() << std::endl;
}

void RunArgumentMode(char* argv[])
{
	const int upperBound = ParseUpperBound(argv[1]);
	PrintPrimes(upperBound);
}

void RunStdinMode()
{
	std::string input;
	if (!std::getline(std::cin, input) || input.empty())
	{
		throw std::runtime_error("upperBound is not provided");
	}

	const int upperBound = ParseUpperBound(input);
	PrintPrimes(upperBound);
}

void ShowHelpMessage()
{
	std::cout
		<< "Usage:" << std::endl
		<< "  primes <upperBound>" << std::endl
		<< "  primes              # interactive stdin mode" << std::endl
		<< "  primes -h | --help  # show this help message" << std::endl
		<< std::endl
		<< "upperBound must be between 0 and 100000000" << std::endl;
}

int main(const int argc, char* argv[])
{
	const AppMode mode = DefineAppMode(argc, argv);

	try
	{
		switch (mode)
		{
		case AppMode::Argument:
			RunArgumentMode(argv);
			break;

		case AppMode::Stdin:
			RunStdinMode();
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
