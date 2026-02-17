#pragma once
#include "TemplateStorage.h"
#include <algorithm>
#include <string>
#include <vector>

inline std::string ExpandTemplate(const std::string& tpl, const std::unordered_map<std::string, std::string>& templates)
{
	std::string result = tpl;

	std::vector<std::pair<std::string, std::string>> entries(templates.begin(), templates.end());

	std::ranges::sort(entries, [](const auto& a, const auto& b) {
		return a.first.size() > b.first.size();
	});

	for (const auto& [key, value] : entries)
	{
		size_t pos = 0;
		while ((pos = result.find(key, pos)) != std::string::npos)
		{
			result.replace(pos, key.size(), value);
			pos += value.size();
		}
	}

	return result;
}
