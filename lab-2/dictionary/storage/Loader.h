#pragma once

#include "IStorage.h"

#include <deque>
#include <memory>
#include <string>

class Loader
{
public:
	explicit Loader(std::string filename);

	std::deque<Translations> LoadData() const;
	void SaveData(std::deque<Translations>& dtos) const;

private:
	std::string m_filename;
};
