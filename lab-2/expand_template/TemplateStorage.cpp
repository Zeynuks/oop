#include "TemplateStorage.h"

std::optional<std::string> TemplateStorage::Find(const std::string& key) const
{
	if (const auto it = storage.find(key); it != storage.end())
	{
		return it->second;
	}

	return std::nullopt;
}

void TemplateStorage::AddTemplate(const std::string& key, const std::string& value)
{
	if (key.empty())
	{
		throw std::invalid_argument("Empty key");
	}

	if (storage.contains(key))
	{
		throw std::invalid_argument("Non-unique key");
	}

	storage.emplace(key, value);
}

const std::unordered_map<std::string, std::string>& TemplateStorage::GetAllTemplates() const
{
	return storage;
}