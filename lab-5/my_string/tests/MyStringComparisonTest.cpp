#include "../MyString.hpp"

#include <gtest/gtest.h>

TEST(MyStringComparisonTest, EqualityAndInequality)
{
	const MyString s1("test");
	const MyString s2("test");
	const MyString s3("temp");
	const MyString s4("testing");

	EXPECT_TRUE(s1 == s2);
	EXPECT_FALSE(s1 == s3);
	EXPECT_FALSE(s1 == s4);

	EXPECT_TRUE(s1 != s3);
	EXPECT_FALSE(s1 != s2);
}

TEST(MyStringComparisonTest, RelationalOperators)
{
	const MyString a("apple");
	const MyString b("apply");
	const MyString c("apples");

	EXPECT_TRUE(a < b);
	EXPECT_TRUE(a < c);
	EXPECT_TRUE(b > a);
	EXPECT_TRUE(c > a);
	EXPECT_TRUE(a <= b);
	EXPECT_TRUE(a <= a);
	EXPECT_TRUE(b >= a);
	EXPECT_TRUE(c >= a);
}

TEST(MyStringComparisonTest, ComparisonWithEmbeddedNulls)
{
	char buf1[] = {'A', '\0', '1'};
	char buf2[] = {'A', '\0', '2'};
	char buf3[] = {'A', '\0', '1'};

	const MyString s1(buf1, 3);
	const MyString s2(buf2, 3);
	const MyString s3(buf3, 3);

	EXPECT_TRUE(s1 == s3);
	EXPECT_FALSE(s1 == s2);
	EXPECT_TRUE(s1 < s2);
	EXPECT_TRUE(s2 > s1);
}

TEST(MyStringComparisonTest, ComparisonWithEmptyStrings)
{
	const MyString empty1;
	const MyString empty2("");
	const MyString notEmpty("a");

	EXPECT_TRUE(empty1 == empty2);
	EXPECT_TRUE(empty1 < notEmpty);
	EXPECT_FALSE(notEmpty < empty1);
}