#include "InMemoryStorage.h"

Entry& InMemoryStorage::Add(const std::string& word)
{
	m_entries.emplace_back();
	Entry& entry = m_entries.back();
	entry.word = word;

	m_wordIndex[word] = &entry;
	return entry;
}

Entry& InMemoryStorage::Get(const std::string& word) const
{
	const auto it = m_wordIndex.find(word);
	if (it == m_wordIndex.end())
	{
		throw std::out_of_range("Word not found");
	}

	return *it->second;
}
void InMemoryStorage::Load(const std::vector<Translations>& translations)
{
	for (const auto& [word, translation] : translations)
	{
		Entry* word_entry = nullptr;
		Entry* translation_entry = nullptr;

		auto it_word = m_wordIndex.find(word);
		if (it_word == m_wordIndex.end())
		{
			word_entry = &Add(word);
		}
		else
		{
			word_entry = it_word->second;
		}

		auto it_translation = m_wordIndex.find(translation);
		if (it_translation == m_wordIndex.end())
		{
			translation_entry = &Add(translation);
		}
		else
		{
			translation_entry = it_translation->second;
		}

		word_entry->translations.insert(translation_entry);
		translation_entry->translations.insert(word_entry);
	}
}

std::vector<Translations> InMemoryStorage::Upload() const
{
	std::vector<Translations> result;

	for (const auto& [word, translations] : m_entries)
	{
		for (const Entry* translation : translations)
		{
			if (word < translation->word)
			{
				result.emplace_back(word, translation->word);
			}
		}
	}

	return result;
}
