#include "../parser.h"
#include <gtest/gtest.h>
#include <string>

TEST(TryParseURL, ValidHttpUrlWithoutPortAndDocument)
{
    const std::string url = "http://example.com";
	const UrlData expected{
        "http",
        "example.com",
        80,
        ""
    };

    UrlData actual;
    EXPECT_TRUE(TryParseURL(url, actual));
    EXPECT_EQ(expected, actual);
}

TEST(TryParseURL, ValidHttpsUrlWithPortAndDocument)
{
    const std::string url = "https://example.com:8080/index.html";
	const UrlData expected{
        "https",
        "example.com",
        8080,
        "index.html"
    };

    UrlData actual;
    EXPECT_TRUE(TryParseURL(url, actual));
    EXPECT_EQ(expected, actual);
}

TEST(TryParseURL, ValidFtpUrlWithDocument)
{
    const std::string url = "ftp://ftp.example.com/files/file.txt";
	const UrlData expected{
        "ftp",
        "ftp.example.com",
        21,
        "files/file.txt"
    };

    UrlData actual;
    EXPECT_TRUE(TryParseURL(url, actual));
    EXPECT_EQ(expected, actual);
}

TEST(TryParseURL, ValidUrlWithMixedCaseProtocol)
{
    const std::string url = "HtTpS://example.com";
	const UrlData expected{
        "https",
        "example.com",
        443,
        ""
    };

    UrlData actual;
    EXPECT_TRUE(TryParseURL(url, actual));
    EXPECT_EQ(expected, actual);
}

TEST(TryParseURL, InvalidProtocol)
{
    const std::string url = "htp://example.com";
    UrlData data;
    EXPECT_FALSE(TryParseURL(url, data));
}

TEST(TryParseURL, MissingProtocolSeparator)
{
    const std::string url = "http//example.com";
    UrlData data;
    EXPECT_FALSE(TryParseURL(url, data));
}

TEST(TryParseURL, InvalidPortNumber)
{
    const std::string url = "http://example.com:0";
    UrlData data;
    EXPECT_FALSE(TryParseURL(url, data));
}

TEST(TryParseURL, PortNumberOutOfRange)
{
    const std::string url = "http://example.com:65536";
    UrlData data;
    EXPECT_FALSE(TryParseURL(url, data));
}

TEST(TryParseURL, MissingHost)
{
    const std::string url = "http://:8080";
    UrlData data;
    EXPECT_FALSE(TryParseURL(url, data));
}

TEST(TryParseURL, EmptyString)
{
    const std::string url = "";
    UrlData data;
    EXPECT_FALSE(TryParseURL(url, data));
}

TEST(TryParseURL, ValidUrlWithMaxPortNumber)
{
    const std::string url = "http://example.com:65535";
	const UrlData expected{
        "http",
        "example.com",
        65535,
        ""
    };

    UrlData actual;
    EXPECT_TRUE(TryParseURL(url, actual));
    EXPECT_EQ(expected, actual);
}

TEST(TryParseURL, ValidUrlWithMinPortNumber)
{
    const std::string url = "http://example.com:1";
	const UrlData expected{
        "http",
        "example.com",
        1,
        ""
    };

    UrlData actual;
    EXPECT_TRUE(TryParseURL(url, actual));
    EXPECT_EQ(expected, actual);
}

TEST(TryParseURL, UrlWithEmptyDocument)
{
    const std::string url = "http://example.com/";
	const UrlData expected{
        "http",
        "example.com",
        80,
        ""
    };

    UrlData actual;
    EXPECT_TRUE(TryParseURL(url, actual));
    EXPECT_EQ(expected, actual);
}

TEST(TryParseURL, UrlWithOnlyProtocolAndHost)
{
    const std::string url = "ftp://host";
	const UrlData expected{
        "ftp",
        "host",
        21,
        ""
    };

    UrlData actual;
    EXPECT_TRUE(TryParseURL(url, actual));
    EXPECT_EQ(expected, actual);
}

TEST(TryParseURL, UrlWithTrailingSlashAfterPort)
{
    const std::string url = "http://example.com:8080/";
	const UrlData expected{
        "http",
        "example.com",
        8080,
        ""
    };

    UrlData actual;
    EXPECT_TRUE(TryParseURL(url, actual));
    EXPECT_EQ(expected, actual);
}

TEST(TryParseURL, InvalidPortFormat)
{
    const std::string url = "http://example.com:port";
    UrlData data;
    EXPECT_FALSE(TryParseURL(url, data));
}