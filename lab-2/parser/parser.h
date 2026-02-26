#pragma once
#include <string>

struct UrlData
{
	std::string protocol;
	std::string host;
	int port;
	std::string document;

	bool operator==(const UrlData& rhs) const = default;
};

bool TryParseURL(std::string_view url, UrlData& result);