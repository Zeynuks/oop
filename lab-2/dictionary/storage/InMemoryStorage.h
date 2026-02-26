#pragma once
#include "../core/Entry.h"
#include "IStorage.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class InMemoryStorage final : public IStorage
{
public:
	Entry* Add(const std::string& word) override;
	[[nodiscard]] Entry* Get(const std::string& word) const override;

	void Load(const std::deque<Translations>& dtos) override;
	std::deque<Translations> Upload() const override;

private:
	std::unordered_map<std::string, Entry*> m_wordIndex;
	std::vector<std::unique_ptr<Entry>> m_entries;
};
