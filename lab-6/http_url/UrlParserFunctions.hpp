#pragma once
#include <algorithm>
#include <regex>
#include <string>

enum class Protocol
{
	HTTP,
	HTTPS,
	FTP
};

using Port = unsigned short;

struct UrlData
{
	Protocol protocol;
	std::string domain;
	Port port;
	std::string document;

	bool operator==(const UrlData& rhs) const = default;
};

namespace UrlParserFunctions
{
constexpr Port minPort = 1;
constexpr Port maxPort = 65535;
const std::regex urlRegex(R"(^([a-zA-Z]+):\/\/([a-zA-Z0-9\.\-]+)(?::([0-9]{1,5}))?(?:\/([^?#\s]*))?)", std::regex::icase);

struct ProtocolInfo
{
	Protocol value;
	std::string name;
	Port defaultPort;
};

constexpr std::array PROTOCOLS = {
	ProtocolInfo{ Protocol::HTTP, "http", 80 },
	ProtocolInfo{ Protocol::HTTPS, "https", 443 },
	ProtocolInfo{ Protocol::FTP, "ftp", 21 }
};

inline std::string ToLower(const std::string& str)
{
	std::string result(str);

	std::ranges::transform(result, result.begin(),
		[](const unsigned char c) {
			return static_cast<char>(std::tolower(c));
		});

	return result;
}

inline std::string Trim(const std::string& str)
{
	const auto start = std::ranges::find_if_not(str, [](unsigned char c) {
		return std::isspace(c);
	});

	if (start == str.end())
	{
		return {};
	}

	const auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c) {
		return std::isspace(c);
	}).base();

	return str.substr(std::distance(str.begin(), start), std::distance(start, end));
}

inline bool ContainsSpaces(const std::string& str)
{
	return std::ranges::any_of(str, [](const unsigned char c) {
		return std::isspace(c);
	});
}

inline bool IsPortValid(const Port port)
{
	if (port < minPort || port > maxPort)
	{
		return false;
	}

	return true;
}

inline bool TryParseProtocol(const std::string& str, Protocol& protocol)
{
	const std::string lower = ToLower(str);

	for (const auto& [value, name, defaultPort] : PROTOCOLS)
	{
		if (name == lower)
		{
			protocol = value;
			return true;
		}
	}

	return false;
}

inline bool TryParseDomain(const std::string& domainStr, std::string& domain)
{
	const std::string trimmed = Trim(domainStr);

	if (trimmed.empty())
	{
		return false;
	}

	domain = std::move(trimmed);
	return true;
}

inline bool TryGetDefaultPort(const Protocol protocol, Port& port)
{
	for (const auto& [value, name, defaultPort] : PROTOCOLS)
	{
		if (value == protocol)
		{
			port = defaultPort;
			return true;
		}
	}

	return false;
}

inline bool TryParsePort(const std::string& portStr, const Protocol protocol, Port& port)
{
	const std::string trimmed = Trim(portStr);

	if (trimmed.empty())
	{
		return TryGetDefaultPort(protocol, port);
	}

	unsigned int value = 0;
	auto [ptr, ec] = std::from_chars(trimmed.data(), trimmed.data() + trimmed.size(), value);

	if (ec != std::errc{} || ptr != trimmed.data() + trimmed.size())
	{
		return false;
	}

	if (!IsPortValid(value))
	{
		return false;
	}

	port = static_cast<Port>(value);
	return true;
}

inline bool TryParseDocument(const std::string& documentStr, std::string& result)
{
	std::string trimmed = Trim(documentStr);

	if (trimmed.empty())
	{
		result = "/";
		return true;
	}

	if (ContainsSpaces(trimmed))
	{
		return false;
	}

	if (trimmed[0] != '/')
	{
		result = "/" + std::string(trimmed);
	}
	else
	{
		result = std::string(trimmed);
	}

	return true;
}

inline bool TryParseURL(const std::string& url, UrlData& result)
{
	UrlData temp;

	if (ContainsSpaces(url))
	{
		return false;
	}

	std::smatch match;

	if (!std::regex_match(url.begin(), url.end(), match, urlRegex))
	{
		return false;
	}

	if (!TryParseProtocol(match[1].str(), temp.protocol))
	{
		return false;
	}

	if (!TryParseDomain(match[2].str(), temp.domain))
	{
		return false;
	}

	if (!TryParsePort(match[3].str(), temp.protocol, temp.port))
	{
		return false;
	}

	if (!TryParseDocument(match[4].str(), temp.document))
	{
		return false;
	}

	result = std::move(temp);

	return true;
}

inline std::string ToString(const Protocol p)
{
	const auto it = std::ranges::find_if(PROTOCOLS,
		[p](const ProtocolInfo& info) {
			return info.value == p;
		});

	if (it != PROTOCOLS.end())
	{
		return std::string(it->name);
	}

	return "unknown";
}
} // namespace UrlParserFunctions