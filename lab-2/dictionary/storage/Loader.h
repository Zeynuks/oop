#pragma once

#include "IStorage.h"

#include <deque>
#include <memory>
#include <string>

class Loader
{
public:
	explicit Loader(std::string filename);

	std::deque<EntryDto> LoadData() const;
	void SaveData(std::deque<EntryDto>& dtos) const;

private:
	std::string m_filename;
};
