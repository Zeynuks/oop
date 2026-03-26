#include "../html_decode.h"
#include <gtest/gtest.h>

TEST(HtmlDecodeTests, DecodesSingleHtmlEntity)
{
	EXPECT_EQ(HtmlDecode("&quot;"), "\"");
	EXPECT_EQ(HtmlDecode("&apos;"), "'");
	EXPECT_EQ(HtmlDecode("&lt;"), "<");
	EXPECT_EQ(HtmlDecode("&gt;"), ">");
	EXPECT_EQ(HtmlDecode("&amp;"), "&");
}

TEST(HtmlDecodeTests, DecodesMultipleHtmlEntities)
{
	EXPECT_EQ(HtmlDecode("&lt;div&gt;Hello &amp; Welcome&lt;/div&gt;"), "<div>Hello & Welcome</div>");
	EXPECT_EQ(HtmlDecode("&quot;Test&quot; &apos;String&apos;"), "\"Test\" 'String'");
}

TEST(HtmlDecodeTests, IgnoresInvalidHtmlEntities)
{
	EXPECT_EQ(HtmlDecode("&sus;"), "&sus;");
	EXPECT_EQ(HtmlDecode("&amogus"), "&amogus");
	EXPECT_EQ(HtmlDecode("text&amogus;text"), "text&amogus;text");
}

TEST(HtmlDecodeTests, HandlesMixedValidAndInvalidEntities)
{
	EXPECT_EQ(HtmlDecode("&lt;div&gt;mama: &yanaoop;&lt;/div&gt;"), "<div>mama: &yanaoop;</div>");
}

TEST(HtmlDecodeTests, HandlesEmptyString)
{
	EXPECT_EQ(HtmlDecode(""), "");
}

TEST(HtmlDecodeTests, HandlesStringWithoutEntities)
{
	EXPECT_EQ(HtmlDecode("abcdefghijklmnopqrstuvwxyz"), "abcdefghijklmnopqrstuvwxyz");
	EXPECT_EQ(HtmlDecode("0123456789"), "0123456789");
}

TEST(HtmlDecodeTests, HandlesStringWithOnlyAmpersand)
{
	EXPECT_EQ(HtmlDecode("&"), "&");
	EXPECT_EQ(HtmlDecode("&&"), "&&");
}

TEST(HtmlDecodeTests, HandlesLongHtmlString)
{
	EXPECT_EQ(HtmlDecode("&&&&asus;;;;;;"), "&&&&asus;;;;;;");
	EXPECT_EQ(HtmlDecode("&&&&quot;;;;;"), "&&&\";;;;");
	EXPECT_EQ(HtmlDecode("&&apos;;"), "&\';");
	EXPECT_EQ(HtmlDecode("&lt;;;;;;;"), "<;;;;;;");
	EXPECT_EQ(HtmlDecode("&&&&&lt;"), "&&&&<");
}

TEST(HtmlDecodeTests, HandlesLongHtmlStringWithEntities)
{
	EXPECT_EQ(HtmlDecode("&lt;Tag&gt; &quot;Quoted&quot; &apos;Text&apos; &amp;&amp; &amp;sus;"), "<Tag> \"Quoted\" 'Text' && &sus;");
}