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

	int result = 0;

	for (; index < str.size(); ++index)
	{
		const char currentChar = str[index];
		if (!IsValidCharacter(currentChar, radix))
		{
			throw std::invalid_argument("invalid character in input");
		}

		const int digit = CharToDigit(currentChar);
		if (!isNegative)
		{
			if (result > (INT_MAX - digit) / radix)
			{
				throw std::out_of_range("value out of range for int");
			}

			result = result * radix + digit;
		}
		else
		{
			if (result < (INT_MIN + digit) / radix)
			{
				throw std::out_of_range("value out of range for int");
			}

			result = result * radix - digit;
		}
	}

	return result;
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
	int absNumber = number;

	while (absNumber != 0)
	{
		int digit = absNumber % radix;
		if (digit < 0)
		{
			digit = -digit;
		}

		if (digit < 10)
		{
			result += static_cast<char>('0' + digit);
		}
		else
		{
			result += static_cast<char>('A' + (digit - 10));
		}

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