#include "replace_core.h"

std::string ReplaceString(const std::string& source,
	const std::string& search,
	const std::string& replace)
{
	if (search.empty())
	{
		return source;
	}

	std::string result;
	size_t currentPos = 0;
	size_t foundPos = 0;

	while ((foundPos = source.find(search, currentPos)) != std::string::npos)
	{
		result.append(source, currentPos, foundPos - currentPos);
		result += replace;
		currentPos = foundPos + search.length();
	}
	result.append(source, currentPos);

	return result;
}

std::string ReplaceStringWithRanges(const std::string& source,
	const std::string& search,
	const std::string& replace)
{
	if (search.empty())
	{
		return source;
	}

	return std::views::split(source, search)
		| std::views::join_with(replace)
		| std::ranges::to<std::string>();
}

std::string ReplaceStringWithSearch(const std::string& source,
	const std::string& search,
	const std::string& replace)
{
	if (search.empty())
	{
		return source;
	}

	std::string result;
	result.reserve(source.size());

	const auto searcher = std::boyer_moore_searcher(search.begin(), search.end());
	auto it = source.begin();
	while (true)
	{
		auto found = std::search(it, source.end(), searcher);
		if (found == source.end())
		{
			break;
		}

		result.append(it, found);
		result += replace;
		it = found + search.size();
	}
	result.append(it, source.end());
	return result;
}