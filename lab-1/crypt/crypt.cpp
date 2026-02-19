#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

namespace bytes
{
constexpr unsigned char FIRST = 0b00000001;
constexpr unsigned char SECOND = 0b00000010;
constexpr unsigned char THIRD = 0b00000100;
constexpr unsigned char FOURTH = 0b00001000;
constexpr unsigned char FIFTH = 0b00010000;
constexpr unsigned char SIXTH = 0b00100000;
constexpr unsigned char SEVENTH = 0b01000000;
constexpr unsigned char EIGHTH = 0b10000000;
} // namespace bytes

const std::string CRYPT = "crypt";
const std::string DECRYPT = "decrypt";
constexpr int DECIMAL = 10;
constexpr int MIN_KEY = 0;
constexpr int MAX_KEY = 255;
constexpr int ARGC_COUNT = 5;

enum class Mode
{
	Crypt,
	Decrypt
};

struct Arguments
{
	Mode mode;
	std::ifstream inputFile;
	std::ofstream outputFile;
	unsigned char key;
};

Mode TryParseMode(const std::string& mode)
{
	if (mode == "crypt")
	{
		return Mode::Crypt;
	}

	if (mode == "decrypt")
	{
		return Mode::Decrypt;
	}

	throw std::invalid_argument("Некорректный режим!");
}

int TryParseKey(const std::string& key)
{
	const int value = std::stoi(key);

	if (value < MIN_KEY || value > MAX_KEY)
	{
		throw std::out_of_range("Ключ должен быть от 0 до 255!");
	}

	return value;
}

template <typename T>
concept FileStream = std::same_as<T, std::ifstream> || std::same_as<T, std::ofstream>;

template <FileStream T>
T TryOpenFile(const std::string& filename)
{
	T file(filename);
	if (!file)
	{
		throw std::runtime_error("Не удалось открыть файл: " + filename + '!');
	}

	return file;
}

Arguments TryParseArguments(int argc, char* argv[])
{
	Arguments arguments;

	if (argc != ARGC_COUNT)
	{
		throw std::invalid_argument("Программа ожидает: start crypt <input file> <output file> <key> ");
	}

	arguments.mode = TryParseMode(argv[1]);
	arguments.key = TryParseKey(argv[4]);
	arguments.inputFile = TryOpenFile<std::ifstream>(argv[2]);
	arguments.outputFile = TryOpenFile<std::ofstream>(argv[3]);

	return arguments;
}

unsigned char GetCryptedChar(const unsigned char ch, const unsigned char key)
{
	unsigned char outChar = 0;
	const unsigned char cryptByte = ch ^ key;
	outChar |= (cryptByte & bytes::EIGHTH) >> 2;
	outChar |= (cryptByte & bytes::SEVENTH) >> 5;
	outChar |= (cryptByte & bytes::SIXTH) >> 5;
	outChar |= (cryptByte & bytes::FIFTH) << 3;
	outChar |= (cryptByte & bytes::FOURTH) << 3;
	outChar |= (cryptByte & bytes::THIRD) << 2;
	outChar |= (cryptByte & bytes::SECOND) << 2;
	outChar |= (cryptByte & bytes::FIRST) << 2;

	return outChar;
}

unsigned char GetDecryptedChar(const unsigned char ch, const unsigned char key)
{
	unsigned char decryptChar = 0;
	decryptChar |= (ch & bytes::EIGHTH) >> 3;
	decryptChar |= (ch & bytes::SEVENTH) >> 3;
	decryptChar |= (ch & bytes::SIXTH) << 2;
	decryptChar |= (ch & bytes::FIFTH) >> 2;
	decryptChar |= (ch & bytes::FOURTH) >> 2;
	decryptChar |= (ch & bytes::THIRD) >> 2;
	decryptChar |= (ch & bytes::SECOND) << 5;
	decryptChar |= (ch & bytes::FIRST) << 5;

	return decryptChar ^ key;
}

void Crypt(std::istream& input, std::ostream& output, const unsigned char key)
{
	char byte;
	while (input.get(byte))
	{
		output << (GetCryptedChar((byte), key));
	}
}

void Decrypt(std::istream& input, std::ostream& output, const unsigned char key)
{
	char byte;
	while (input.get(byte))
	{
		output << (GetDecryptedChar((byte), key));
	}
}

int main(const int argc, char* argv[])
{

	try
	{
		Arguments arguments = TryParseArguments(argc, argv);
		if (arguments.mode == Mode::Crypt)
		{
			Crypt(arguments.inputFile, arguments.outputFile, arguments.key);
		}
		if (arguments.mode == Mode::Decrypt)
		{
			Decrypt(arguments.inputFile, arguments.outputFile, arguments.key);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}