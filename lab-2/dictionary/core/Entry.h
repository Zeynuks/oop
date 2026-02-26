#pragma once
#include <string>
#include <unordered_set>

struct Entry
{
	std::string word;
	std::unordered_set<Entry*> translations;
};