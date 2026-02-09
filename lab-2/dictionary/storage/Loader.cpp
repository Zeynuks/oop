#include "Loader.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

Loader::Loader(std::string filename)
	: m_filename(std::move(filename))
{
}

std::deque<std::pair<std::string, std::string>> Loader::LoadData() const
{
	if (!std::filesystem::exists(m_filename))
	{
		if (std::filesystem::path filepath(m_filename); !std::filesystem::exists(filepath.parent_path()))
		{
			std::filesystem::create_directories(filepath.parent_path());
		}

		if (std::ofstream newFile(m_filename); !newFile.is_open())
		{
			throw std::runtime_error("Cannot create new dictionary file: " + m_filename);
		}

		std::cout << "Файл словаря не найден. Создан новый файл: " << m_filename << std::endl; // Нужно ли
	}

	std::ifstream file(m_filename);
	if (!file.is_open())
	{
		throw std::runtime_error("Cannot open dictionary file: " + m_filename);
	}

	std::deque<std::pair<std::string, std::string>> words;

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

void Loader::SaveData(std::deque<std::pair<std::string, std::string>>& words) const
{
	std::ofstream file(m_filename, std::ios::trunc);
	if (!file.is_open())
	{
		throw std::runtime_error("Cannot open file for saving: " + m_filename);
	}

	for (const auto& [word, translation] : words)
	{
		file << word << " " << translation << std::endl;
	}
}