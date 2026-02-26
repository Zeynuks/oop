#include "IStorage.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Loader
{
std::vector<Translations> LoadData(std::string filename)
{
	if (!std::filesystem::exists(filename))
	{
		if (std::filesystem::path filepath(filename); !std::filesystem::exists(filepath.parent_path()))
		{
			std::filesystem::create_directories(filepath.parent_path());
		}

		if (std::ofstream newFile(filename); !newFile.is_open())
		{
			throw std::runtime_error("Cannot create new dictionary file: " + filename);
		}

		std::cout << "Файл словаря не найден. Создан новый файл: " << filename << std::endl; // Нужно ли
	}

	std::ifstream file(filename);
	if (!file.is_open())
	{
		throw std::runtime_error("Cannot open dictionary file: " + filename);
	}

	std::vector<std::pair<std::string, std::string>> words;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}

		std::istringstream iss(line);
		std::string word;
		std::string translation;

		iss >> word >> translation;

		if (word.empty() || translation.empty())
		{
			throw std::invalid_argument("Invalid word or translation");
		}

		words.emplace_back(word, translation);
	}
	return words;
}

void SaveData(const std::string& filename, std::vector<Translations> words)
{
	std::ofstream file(filename, std::ios::trunc);
	if (!file.is_open())
	{
		throw std::runtime_error("Cannot open file for saving: " + filename);
	}

	for (const auto& [word, translation] : words)
	{
		file << word << " " << translation << std::endl;
	}
}
} // namespace Loader