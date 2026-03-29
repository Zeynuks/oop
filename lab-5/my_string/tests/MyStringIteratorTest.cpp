#include "../MyString.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

TEST(MyStringIteratorTest, BeginEndIteration)
{
	MyString str("Iter");
	std::string result;
	for (auto it = str.begin(); it != str.end(); ++it)
	{
		result += *it;
	}
	EXPECT_EQ(result, "Iter");
}

TEST(MyStringIteratorTest, ConstBeginEndIteration)
{
	const MyString str("Const");
	std::string result;
	for (auto it = str.begin(); it != str.end(); ++it)
	{
		result += *it;
	}
	EXPECT_EQ(result, "Const");
}

TEST(MyStringIteratorTest, RangeBasedForLoop)
{
	MyString str("Range");
	std::string result;
	for (const char c : str)
	{
		result += c;
	}
	EXPECT_EQ(result, "Range");
}

TEST(MyStringIteratorTest, IteratorModification)
{
	MyString str("test");
	const auto it = str.begin();
	*it = 'T';
	EXPECT_STREQ(str.GetStringData(), "Test");
}

TEST(MyStringIteratorTest, ReverseIteration)
{
	MyString str("Reverse");
	std::string result;
	for (auto it = str.rbegin(); it != str.rend(); ++it)
	{
		result += *it;
	}
	EXPECT_EQ(result, "esreveR");
}

TEST(MyStringIteratorTest, ConstReverseIteration)
{
	const MyString str("Back");
	std::string result;
	for (auto it = str.crbegin(); it != str.crend(); ++it)
	{
		result += *it;
	}
	EXPECT_EQ(result, "kcaB");
}

TEST(MyStringIteratorTest, IteratorArithmetic)
{
	MyString str("Arithmetic");
	auto it = str.begin();

	EXPECT_EQ(*(it + 3), 't');
	EXPECT_EQ(*(it + 4), 'h');

	EXPECT_EQ(*(str.end() - 1), 'c');

	const auto it2 = it + 2;
	EXPECT_EQ(it2 - it, 2);

	it += 1;
	EXPECT_EQ(*it, 'r');
}

TEST(MyStringIteratorTest, IteratorComparison)
{
	MyString str("Compare");
	const auto it1 = str.begin();
	const auto it2 = str.begin() + 3;

	EXPECT_TRUE(it1 < it2);
	EXPECT_TRUE(it2 > it1);
	EXPECT_TRUE(it1 <= it1);
	EXPECT_TRUE(it2 >= it1);
	EXPECT_FALSE(it1 == it2);
	EXPECT_TRUE(it1 != it2);
}

TEST(MyStringIteratorTest, STLAlgorithmCompatibility)
{
	MyString str("dcba");
	std::ranges::sort(str);
	EXPECT_STREQ(str.GetStringData(), "abcd");

	const auto it = std::ranges::find(str, 'b');
	EXPECT_NE(it, str.end());
	EXPECT_EQ(*it, 'b');
}

TEST(MyStringIteratorTest, IteratorWithEmbeddedNulls)
{
	char buf[] = { 'A', '\0', 'B' };
	MyString str(buf, 3);

	std::vector<char> result;
	for (char c : str)
	{
		result.push_back(c);
	}

	ASSERT_EQ(result.size(), 3);
	EXPECT_EQ(result[0], 'A');
	EXPECT_EQ(result[1], '\0');
	EXPECT_EQ(result[2], 'B');
}