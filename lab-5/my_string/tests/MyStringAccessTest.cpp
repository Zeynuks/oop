#include "../MyString.hpp"

#include <gtest/gtest.h>
#include <sstream>

TEST(MyStringAccessTest, OperatorBracket_Read)
{
	const MyString str("ReadOnly");
	EXPECT_EQ(str[0], 'R');
	EXPECT_EQ(str[4], 'O');
	EXPECT_EQ(str[7], 'y');
	EXPECT_EQ(str[8], '\0');
}

TEST(MyStringAccessTest, OperatorBracket_Write)
{
	MyString str("Base");
	str[0] = 'C';
	str[3] = 'k';

	EXPECT_STREQ(str.GetStringData(), "Cask");
	EXPECT_EQ(str.GetLength(), 4);
}

TEST(MyStringAccessTest, OperatorBracket_EmbeddedNulls)
{
	char buf[] = { 'A', '\0', 'B' };
	MyString str(buf, 3);

	EXPECT_EQ(str[0], 'A');
	EXPECT_EQ(str[1], '\0');
	EXPECT_EQ(str[2], 'B');

	str[1] = 'X';
	EXPECT_EQ(str[1], 'X');
	EXPECT_STREQ(str.GetStringData(), "AXB");
}

TEST(MyStringIOTest, OutputOperator_Normal)
{
	const MyString str("Hello Stream");
	std::stringstream oss;
	oss << str;
	EXPECT_EQ(oss.str(), "Hello Stream");
}

TEST(MyStringIOTest, OutputOperator_WithEmbeddedNulls)
{
	char buf[] = { 'H', '\0', 'i' };
	const MyString str(buf, 3);
	std::stringstream oss;
	oss << str;

	const std::string res = oss.str();
	EXPECT_EQ(res.length(), 3);
	EXPECT_EQ(res[1], '\0');
}

TEST(MyStringIOTest, InputOperator_Basic)
{
	std::stringstream iss("Word1 Word2");
	MyString str;

	iss >> str;
	EXPECT_STREQ(str.GetStringData(), "Word1");

	iss >> str;
	EXPECT_STREQ(str.GetStringData(), "Word2");
}

TEST(MyStringIOTest, InputOperator_Empty)
{
	std::stringstream iss("");
	MyString str("NonEmpty");

	iss >> str;
	EXPECT_EQ(str.GetLength(), 0);
}

TEST(MyStringIOTest, Getline_Basic)
{
	std::stringstream iss("Hello World with Spaces\nNext Line");
	MyString str;

	GetLine(iss, str);
	EXPECT_STREQ(str.GetStringData(), "Hello World with Spaces");
	EXPECT_EQ(str.GetLength(), 23);

	GetLine(iss, str);
	EXPECT_STREQ(str.GetStringData(), "Next Line");
}

TEST(MyStringIOTest, Getline_CustomDelimiter)
{
	std::stringstream iss("Part1;Part2;Part3");
	MyString str;

	GetLine(iss, str, ';');
	EXPECT_STREQ(str.GetStringData(), "Part1");

	GetLine(iss, str, ';');
	EXPECT_STREQ(str.GetStringData(), "Part2");
}

TEST(MyStringIOTest, Getline_EmptyAndOnlyDelimiter)
{
	std::stringstream iss1("");
	MyString str1("Initial");
	GetLine(iss1, str1);
	EXPECT_EQ(str1.GetLength(), 0);

	std::stringstream iss2("\n\n");
	MyString str2("Initial");

	GetLine(iss2, str2);
	EXPECT_EQ(str2.GetLength(), 0);

	GetLine(iss2, str2);
	EXPECT_EQ(str2.GetLength(), 0);
}

TEST(MyStringIOTest, Getline_EmbeddedNulls)
{
	std::string data = "Start";
	data.push_back('\0');
	data += "End\nNext";

	std::stringstream iss(data);
	MyString str;

	GetLine(iss, str);

	EXPECT_EQ(str.GetLength(), 9);
	EXPECT_EQ(str[5], '\0');
	EXPECT_EQ(str[8], 'd');
}