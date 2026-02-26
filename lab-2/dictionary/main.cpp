#include "core/Dictionary.h"
#include "storage/InMemoryStorage.h"
#include "storage/Loader.cpp"

#include <iostream>
#include <string>

constexpr std::string dictionaryFileName = "data/dict.dat";

void PrintTranslations(const std::unordered_set<std::string>& translations)
{
	for (auto it = translations.begin(); it != translations.end(); ++it)
	{
		if (it != translations.begin())
		{
			std::cout << ", ";
		}
		std::cout << *it;
	}
	std::cout << std::endl;
}

bool HandleUnknownWord(const Dictionary& dictionary, const std::string& word)
{
	std::cout << "Неизвестное слово \"" << word
			  << "\". Введите перевод или пустую строку для отказа:" << std::endl;

	std::string value;
	std::getline(std::cin, value);

	if (!value.empty())
	{
		dictionary.AddWord(word, value);
		std::cout << "Слово \"" << word << "\" сохранено как \"" << value << "\"." << std::endl;
		return true;
	}

	std::cout << "Слово \"" << word << "\" проигнорировано." << std::endl;
	return false;
}

bool AskSaveChanges()
{
	std::cout << "В словарь были внесены изменения. Введите Y или y для сохранения:" << std::endl;
	std::string answer;
	std::getline(std::cin, answer);
	return answer == "Y" || answer == "y";
}

void RunDictionary(const Dictionary& dictionary, const InMemoryStorage& storage)
{
	bool changed = false;
	std::string input;

	while (true)
	{
		std::cout << "> ";
		if (!std::getline(std::cin, input) || input.empty())
		{
			break;
		}

		try
		{
			if (auto translations = dictionary.GetTranslations(input); !translations.empty())
			{
				PrintTranslations(translations);
				continue;
			}
		}
		catch (...)
		{
		}

		if (HandleUnknownWord(dictionary, input))
		{
			changed = true;
		}
	}

	if (changed && AskSaveChanges())
	{
		const auto translations = storage.Upload();
		Loader::SaveData(dictionaryFileName, translations);
		std::cout << "Изменения сохранены. До свидания." << std::endl;
	}
}

int main()
{
	try
	{
		InMemoryStorage storage;

		const auto translations = Loader::LoadData(dictionaryFileName);
		storage.Load(translations);

		const Dictionary dictionary(storage);

		RunDictionary(dictionary, storage);
	}
	catch (const std::exception& error)
	{
		std::cerr << "ERROR: " << error.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
