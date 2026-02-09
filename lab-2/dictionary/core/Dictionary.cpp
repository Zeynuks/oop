#include "Dictionary.h"
#include "../storage/IStorage.h"

#include <algorithm>
#include <locale>
#include <stdexcept>

Dictionary::Dictionary(IStorage& storage)
	: m_storage(storage)
{
}

void Dictionary::AddWord(const std::string& word) const
{
	const std::string normalized = Normalize(word);

	m_storage.Add(normalized);
}

void Dictionary::AddWord(const std::string& word, const std::string& translation) const
{
	const std::string normalizedWord = Normalize(word);
	const std::string normalizedTranslation = Normalize(translation);

	Entry* wordEntry = m_storage.Get(normalizedWord);
	if (!wordEntry)
	{
		wordEntry = m_storage.Add(normalizedWord);
	}

	Entry* translationEntry = m_storage.Get(normalizedTranslation);
	if (!translationEntry)
	{
		translationEntry = m_storage.Add(normalizedTranslation);
	}

	wordEntry->translations.insert(translationEntry);
	translationEntry->translations.insert(wordEntry);
}

std::unordered_set<std::string> Dictionary::GetTranslations(const std::string& word) const
{
	const std::string normalizedWord = Normalize(word);
	const Entry* wordEntry = m_storage.Get(normalizedWord);

	if (wordEntry == nullptr)
	{
		throw std::invalid_argument("Word not found");
	}

	std::unordered_set<std::string> result;
	result.reserve(wordEntry->translations.size());

	for (const Entry* translations : wordEntry->translations)
	{
		if (translations != nullptr)
		{
			result.insert(translations->word);
		}
	}

	return result;
}

std::string Dictionary::Normalize(const std::string& word)
{
	std::string result = word;

	auto notSpace = [](unsigned char c) {
		return !std::isspace(c);
	};

	result.erase(result.begin(), std::ranges::find_if(result, notSpace));
	result.erase(std::find_if(result.rbegin(), result.rend(), notSpace).base(), result.end());

	for (size_t i = 0; i < result.size(); ++i)
	{
		const unsigned char c = result[i];

		if (c >= 'A' && c <= 'Z')
		{
			result[i] = c - 'A' + 'a';
			continue;
		}

		if (c == 0xD0 && i + 1 < result.size())
		{
			if (const unsigned char c2 = result[i + 1]; c2 >= 0x90 && c2 <= 0xAF)
			{
				result[i + 1] = c2 + 0x20;
			}
		}
	}

	return result;
}
