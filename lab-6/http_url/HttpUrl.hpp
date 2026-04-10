#pragma once

#include "UrlParserFunctions.hpp"

#include <format>
#include <stdexcept>

struct UrlParsingError : std::invalid_argument
{
	UrlParsingError(std::string message)
		: std::invalid_argument(std::move(message))
	{
	}
};

class HttpUrl
{
public:
	HttpUrl(std::string const& url)
	{
		UrlData data;
		if (!UrlParserFunctions::TryParseURL(url, data) || data.protocol == Protocol::FTP)
		{
			throw UrlParsingError("Failed to parse url");
		}

		m_protocol = data.protocol;
		m_domain = data.domain;
		m_document = data.document;
		m_port = data.port;
	}

	HttpUrl(
		std::string const& domain,
		std::string const& document,
		const Protocol protocol = Protocol::HTTP)
		: m_protocol(protocol)
	{
		if (!UrlParserFunctions::TryParseDomain(domain, m_domain))
		{
			throw UrlParsingError("Failed to set domain");
		}

		if (!UrlParserFunctions::TryParseDocument(document, m_document))
		{
			throw UrlParsingError("Invalid document format");
		}

		if (!UrlParserFunctions::TryGetDefaultPort(protocol, m_port))
		{
			throw UrlParsingError("Failed to set default port");
		}
	}

	HttpUrl(
		std::string const& domain,
		std::string const& document,
		const Protocol protocol,
		const Port port)
		: m_protocol(protocol)
	{
		if (!UrlParserFunctions::TryParseDomain(domain, m_domain))
		{
			throw UrlParsingError("Failed to set domain");
		}

		if (!UrlParserFunctions::TryParseDocument(document, m_document))
		{
			throw UrlParsingError("Failed to set document");
		}

		if (!UrlParserFunctions::IsPortValid(port))
		{
			throw UrlParsingError("Failed to set port");
		}

		m_port = port;
	}

	std::string GetURL() const
	{
		return std::format("{}://{}:{}{}", UrlParserFunctions::ToString(m_protocol), m_domain, m_port, m_document);
	}

	std::string GetDomain() const
	{
		return m_domain;
	}

	std::string GetDocument() const
	{
		return m_document;
	}

	Protocol GetProtocol() const
	{
		return m_protocol;
	}

	Port GetPort() const
	{
		return m_port;
	}

	friend std::ostream& operator<<(std::ostream& out, const HttpUrl& url)
	{
		return out << url.GetURL();
	}

private:
	Protocol m_protocol;
	std::string m_domain;
	std::string m_document;
	Port m_port;
};