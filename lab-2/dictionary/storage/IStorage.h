#pragma once
#include "../core/Entry.h"

#include <deque>
#include <string>

using EntryDto = std::pair<std::string, std::string>;

class IStorage
{
public:
	virtual ~IStorage() = default;

	virtual Entry* Add(const std::string& word) = 0;
	[[nodiscard]] virtual Entry* Get(const std::string& word) const = 0;
	virtual void Load(const std::deque<EntryDto>& dtos) = 0;
	virtual std::deque<EntryDto> Upload() const = 0;
};
