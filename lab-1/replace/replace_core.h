#pragma once
#include <ranges>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

std::string ReplaceString(
	const std::string& source,
	const std::string& search,
	const std::string& replace);

std::string ReplaceStringWithRanges(
	const std::string& source,
	const std::string& search,
	const std::string& replace);

std::string ReplaceStringWithSearch(
	const std::string& source,
	const std::string& search,
	const std::string& replace);