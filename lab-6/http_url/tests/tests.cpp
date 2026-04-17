#include "../HttpUrl.hpp"
#include <gtest/gtest.h>

TEST(HttpUrl_ConstructorString, ValidParsing)
{
	const HttpUrl url("http://www.google.com:80/index.html");
	EXPECT_EQ(url.GetProtocol(), Protocol::HTTP);
	EXPECT_EQ(url.GetDomain(), "www.google.com");
	EXPECT_EQ(url.GetPort(), 80);
	EXPECT_EQ(url.GetDocument(), "/index.html");

	const HttpUrl url2("https://github.com/proger/repo");
	EXPECT_EQ(url2.GetProtocol(), Protocol::HTTPS);
	EXPECT_EQ(url2.GetPort(), 443);
	EXPECT_EQ(url2.GetDocument(), "/proger/repo");
}

TEST(HttpUrl_ConstructorString, InvalidParsing)
{
	EXPECT_THROW(HttpUrl(""), UrlParsingError);
	EXPECT_THROW(HttpUrl("ftp://site.com"), UrlParsingError);
	EXPECT_THROW(HttpUrl("http://"), UrlParsingError);
	EXPECT_THROW(HttpUrl("http://:80"), UrlParsingError);
	EXPECT_THROW(HttpUrl("http://site.com:0"), UrlParsingError);
	EXPECT_THROW(HttpUrl("http://site.com:65536"), UrlParsingError);
	EXPECT_THROW(HttpUrl("http://site.com:80abc"), UrlParsingError);
	EXPECT_THROW(HttpUrl("http://site.com:-10"), UrlParsingError);
}

TEST(HttpUrl_ConstructorParts, DomainValidation)
{
	EXPECT_THROW(HttpUrl("", "/index.html"), UrlParsingError);
	EXPECT_THROW(HttpUrl("  ", "/index.html"), UrlParsingError);
}

TEST(HttpUrl_ConstructorParts, DocumentNormalization)
{
	const HttpUrl url1("site.com", "index.html");
	EXPECT_EQ(url1.GetDocument(), "/index.html");

	const HttpUrl url2("site.com", "/index.html");
	EXPECT_EQ(url2.GetDocument(), "/index.html");

	const HttpUrl url3("site.com", "");
	EXPECT_EQ(url3.GetDocument(), "/");
}

TEST(HttpUrl_ConstructorParts, PortValidation)
{
	EXPECT_THROW(HttpUrl("site.com", "/", Protocol::HTTP, 0), UrlParsingError);

	const HttpUrl url("site.com", "/", Protocol::HTTP, 65535);
	EXPECT_EQ(url.GetPort(), 65535);
}

TEST(HttpUrl_GetURL, ProtocolAndPortLogic)
{
	const HttpUrl url1("site.com", "/doc", Protocol::HTTP, 80);
	EXPECT_EQ(url1.GetURL(), "http://site.com:80/doc");

	const HttpUrl url2("site.com", "/doc", Protocol::HTTPS, 443);
	EXPECT_EQ(url2.GetURL(), "https://site.com:443/doc");

	const HttpUrl url3("site.com", "/doc", Protocol::HTTP, 8080);
	EXPECT_EQ(url3.GetURL(), "http://site.com:8080/doc");
}

TEST(HttpUrl_EdgeCases, CaseInsensitivity)
{
	const HttpUrl url("HTTP://SITE.COM/DOC");
	EXPECT_EQ(url.GetProtocol(), Protocol::HTTP);
	EXPECT_EQ(url.GetDomain(), "SITE.COM");
}

TEST(HttpUrl_EdgeCases, MinimalValidUrl)
{
	const HttpUrl url("http://a");
	EXPECT_EQ(url.GetDomain(), "a");
	EXPECT_EQ(url.GetDocument(), "/");
}