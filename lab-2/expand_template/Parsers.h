#pragma once

#include "TemplateStorage.h"

#include <iostream>
#include <string>

inline void ParseTemplates(TemplateStorage& storage)
{
	std::string line;
	while (std::getline(std::cin, line))
	{
		if (line.empty())
		{
			return;
		}

		const auto pos = line.find(":=");
		if (pos == std::string::npos)
		{
			throw std::runtime_error("Invalid input");
		}

		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 2);

		if (!value.empty() && value.front() == ' ')
		{
			value.erase(0, 1);
		}

		while (!key.empty() && key.back() == ' ')
		{
			key.pop_back();
		}

		storage.AddTemplate(key, value);
	}
}

inline void ParseTemplates(const int argc, char* argv[], TemplateStorage& storage)
{
	for (int i = 3; i < argc; i += 2)
	{
		storage.AddTemplate(argv[i], argv[i + 1]);
	}
}
