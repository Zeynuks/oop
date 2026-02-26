#pragma once
#include "../storage/IStorage.h"

#include <unordered_set>

class Dictionary
{
public:
	explicit Dictionary(IStorage& storage);

	void AddWord(const std::string& word, const std::string& translation) const;

	[[nodiscard]] std::unordered_set<std::string> GetTranslations(const std::string& word) const;

private:
	IStorage& m_storage;

	static std::string Normalize(const std::string& word);
};
