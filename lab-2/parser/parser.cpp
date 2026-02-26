#include "parser.h"

#include <algorithm>
#include <regex>

constexpr int minPort = 1;
constexpr int maxPort = 65535;
const std::regex urlRegex(R"(^([a-zA-Z]+):\/\/([a-zA-Z0-9\.\-]+)(?::([0-9]{1,5}))?(?:\/([^?#\s]*))?)", std::regex::icase);

struct ProtocolInfo
{
	std::string_view name;
	int defaultPort;
};

constexpr std::array PROTOCOLS = {
	ProtocolInfo{ "http", 80 },
	ProtocolInfo{ "https", 443 },
	ProtocolInfo{ "ftp", 21 }
};

std::string ToLower(const std::string_view input)
{
	std::string result(input);

	std::ranges::transform(result, result.begin(),
		[](const unsigned char c) {
			return static_cast<char>(std::tolower(c));
		});

	return result;
}

bool TryParseProtocol(const std::string_view input, std::string& protocol)
{
	const std::string lower = ToLower(input);

	for (const auto& [name, defaultPort] : PROTOCOLS)
	{
		if (name == lower)
		{
			protocol = lower;
			return true;
		}
	}

	return false;
}

bool TryGetDefaultPort(const std::string_view protocol, int& port)
{
	for (const auto& [name, defaultPort] : PROTOCOLS)
	{
		if (name == protocol)
		{
			port = defaultPort;
			return true;
		}
	}

	return false;
}

bool TryParsePort(const std::string_view portStr, const std::string_view protocol, int& port)
{
	if (portStr.empty())
	{
		return TryGetDefaultPort(protocol, port);
	}

	int value = 0;

	for (const char c : portStr)
	{
		if (!std::isdigit(static_cast<unsigned char>(c)))
		{
			return false;
		}

		value = value * 10 + (c - '0');

		if (value > maxPort)
		{
			return false;
		}
	}

	if (value < minPort)
	{
		return false;
	}

	port = value;

	return true;
}

bool TryParseURL(const std::string_view url, UrlData& result)
{
	UrlData temp;
	std::cmatch match;

	if (!std::regex_match(url.begin(), url.end(), match, urlRegex))
	{
		return false;
	}

	if (!TryParseProtocol(match[1].str(), temp.protocol))
	{
		return false;
	}

	temp.host = match[2].str();

	if (!TryParsePort(match[3].str(), temp.protocol, temp.port))
	{
		return false;
	}

	temp.document = match[4].matched ? match[4].str() : "";
	result = std::move(temp);

	return true;
}