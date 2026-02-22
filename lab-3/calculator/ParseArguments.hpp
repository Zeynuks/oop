#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

inline void Trim(std::string_view& sv)
{
	auto is_space = [](const char c) {
		return std::isspace(static_cast<unsigned char>(c));
	};

	size_t start = 0;
	size_t end = sv.size();

	while (start < end && is_space(sv[start]))
	{
		++start;
	}
	while (end > start && is_space(sv[end - 1]))
	{
		--end;
	}

	sv = sv.substr(start, end - start);
}

inline void ParseRightPart(std::string_view right, std::vector<std::string>& out)
{
	if (const size_t opPos = right.find_first_of("+-*/"); opPos != std::string_view::npos)
	{
		std::string_view left = right.substr(0, opPos);
		std::string_view op = right.substr(opPos, 1);
		std::string_view right_part = right.substr(opPos + 1);

		Trim(left);
		Trim(right_part);

		if (!left.empty())
		{
			out.emplace_back(left);
		}

		out.emplace_back(op);
		if (!right_part.empty())
		{
			out.emplace_back(right_part);
		}
	}
	else
	{
		Trim(right);
		if (!right.empty())
		{
			out.emplace_back(right);
		}
	}
}

inline std::vector<std::string> ParseArguments(const std::vector<std::string>& args)
{
	std::vector<std::string> result;

	for (auto& s : args)
	{
		std::string_view sv(s);
		Trim(sv);

		if (sv.empty())
		{
			continue;
		}

		if (const size_t eqPos = sv.find('='); eqPos != std::string_view::npos)
		{
			std::string_view left = sv.substr(0, eqPos);
			const std::string_view right = sv.substr(eqPos + 1);

			Trim(left);
			if (!left.empty())
			{
				result.emplace_back(left);
			}

			result.emplace_back("=");

			ParseRightPart(right, result);
		}
		else
		{
			result.emplace_back(sv);
		}
	}

	return result;
}