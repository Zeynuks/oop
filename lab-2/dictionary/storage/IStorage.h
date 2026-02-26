#pragma once
#include "../core/Entry.h"

#include <string>
#include <vector>

using Translations = std::pair<std::string, std::string>;

class IStorage
{
public:
	virtual ~IStorage() = default;

	virtual Entry& Add(const std::string& word) = 0;
	[[nodiscard]] virtual Entry& Get(const std::string& word) const = 0;
	virtual void Load(const std::vector<Translations>& dtos) = 0;
	virtual std::vector<Translations> Upload() const = 0;
};
