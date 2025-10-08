#include <algorithm>
#include <climits>
#include <iostream>
#include <stdexcept>
#include <string>

constexpr int MIN_RADIX = 2;
constexpr int MAX_RADIX = 36;
constexpr int DECIMAL_RADIX = 10;

bool IsValidRadix(const int radix)
{
	return radix >= MIN_RADIX && radix <= MAX_RADIX;
}

bool IsValidCharacter(const char c, const int radix)
{
	if (c >= '0' && c <= '9')
	{
		return (c - '0') < radix;
	}
	if (c >= 'a' && c <= 'z')
	{
		return (10 + (c - 'a')) < radix;
	}
	if (c >= 'A' && c <= 'Z')
	{
		return (10 + (c - 'A')) < radix;
	}
	return false;
}

int CharToDigit(const char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	if (c >= 'a' && c <= 'z')
	{
		return 10 + (c - 'a');
	}
	if (c >= 'A' && c <= 'Z')
	{
		return 10 + (c - 'A');
	}
	throw std::invalid_argument("invalid character in input");
}

int StringToInt(const std::string& str, const int radix)
{
	if (!IsValidRadix(radix))
	{
		throw std::invalid_argument("invalid radix");
	}

	if (str.empty())
	{
		throw std::invalid_argument("empty string");
	}

	const bool isNegative = str[0] == '-';
	size_t index = isNegative ? 1 : 0;

	if (isNegative && str.size() == 1)
	{
		throw std::invalid_argument("invalid string (only '-' sign)");
	}

	long long result = 0;
	const long long maxAllowedValue = isNegative ? -static_cast<long long>(INT_MIN) : INT_MAX;

	for (; index < str.size(); ++index)
	{
		const char currentChar = str[index];
		if (!IsValidCharacter(currentChar, radix))
		{
			throw std::invalid_argument("invalid character in input");
		}

		int digit = CharToDigit(currentChar);
		if (result > (maxAllowedValue - digit) / radix)
		{
			throw std::out_of_range("value out of range for int");
		}

		result = result * radix + digit;
	}

	const long long finalResult = isNegative ? -result : result;

	if (finalResult < INT_MIN || finalResult > INT_MAX)
	{
		throw std::out_of_range("value out of range for int");
	}

	return static_cast<int>(finalResult);
}

std::string IntToString(const int number, const int radix)
{
	if (!IsValidRadix(radix))
	{
		throw std::invalid_argument("invalid radix");
	}

	if (number == 0)
	{
		return "0";
	}

	std::string result;

	long long absNumber = number < 0 ? -(static_cast<long long>(number)) : number;

	while (absNumber != 0)
	{
		const int digit = static_cast<int>(absNumber % radix);
		result += static_cast<char>(digit < 10 ? ('0' + digit) : ('A' + (digit - 10)));
		absNumber /= radix;
	}

	if (number < 0)
	{
		result += '-';
	}

	std::ranges::reverse(result);
	return result;
}

int main(const int argc, char* argv[])
{
	if (argc != 4)
	{
		std::cerr << "Usage: radix.exe <source notation> <destination notation> <value>\n";
		return EXIT_FAILURE;
	}

	try
	{
		const int sourceRadix = StringToInt(argv[1], 10);
		const int destRadix = StringToInt(argv[2], 10);
		const int number = StringToInt(argv[3], sourceRadix);
		std::cout << IntToString(number, destRadix) << "\n";
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}