#include "InMemoryStorage.h"

Entry* InMemoryStorage::Add(const std::string& word)
{
	auto entry = std::make_unique<Entry>();
	entry->word = word;
	m_entries.push_back(std::move(entry));
	Entry* ptr = m_entries.back().get();
	m_wordIndex[word] = ptr;

	return ptr;
}

Entry* InMemoryStorage::Get(const std::string& word) const
{
	const auto it = m_wordIndex.find(word);
	if (it == m_wordIndex.end())
	{
		return nullptr;
	}

	return it->second;
}
void InMemoryStorage::Load(const std::deque<EntryDto>& dtos)
{
	for (const auto& [word, translation] : dtos)
	{
		auto wordEntry = Get(word);
		if (!wordEntry)
		{
			wordEntry = Add(word);
		}

		auto translationEntry = Get(translation);
		if (!translationEntry)
		{
			translationEntry = Add(translation);
		}

		wordEntry->translations.insert(translationEntry);
		translationEntry->translations.insert(wordEntry);
	}
}

std::deque<EntryDto> InMemoryStorage::Upload() const
{
	std::deque<EntryDto> dtos;

	for (const auto& entry : m_entries)
	{
		for (const Entry* translation : entry->translations)
		{
			if (entry->word < translation->word)
			{
				dtos.emplace_back(entry->word, translation->word);
			}
		}
	}

	return dtos;
}
