#include "../MyString.hpp"
#include <gtest/gtest.h>

TEST(MyStringConstructorsTest, DefaultConstructor)
{
	const MyString str;
	EXPECT_EQ(str.GetLength(), 0);
	EXPECT_STREQ(str.GetStringData(), "");
	EXPECT_EQ(str.GetStringData()[0], '\0');
	EXPECT_GE(str.GetCapacity(), 0);
}

TEST(MyStringConstructorsTest, CStringConstructor_Normal)
{
	const MyString str("Hello");
	EXPECT_EQ(str.GetLength(), 5);
	EXPECT_STREQ(str.GetStringData(), "Hello");
}

TEST(MyStringConstructorsTest, CStringConstructor_Empty)
{
	const MyString str("");
	EXPECT_EQ(str.GetLength(), 0);
	EXPECT_STREQ(str.GetStringData(), "");
}

TEST(MyStringConstructorsTest, CStringConstructor_Nullptr)
{
	const MyString str(nullptr);
	EXPECT_EQ(str.GetLength(), 0);
	EXPECT_STREQ(str.GetStringData(), "");
}

TEST(MyStringConstructorsTest, BufferAndLengthConstructor_Normal)
{
	const char* buf = "Hello World";
	const MyString str(buf, 5);
	EXPECT_EQ(str.GetLength(), 5);
	EXPECT_STREQ(str.GetStringData(), "Hello");
}

TEST(MyStringConstructorsTest, BufferAndLengthConstructor_WithEmbeddedNulls)
{
	constexpr char buf[] = { 'A', '\0', 'B', 'C' };
	const MyString str(buf, 4);
	EXPECT_EQ(str.GetLength(), 4);

	const char* data = str.GetStringData();
	EXPECT_EQ(data[0], 'A');
	EXPECT_EQ(data[1], '\0');
	EXPECT_EQ(data[2], 'B');
	EXPECT_EQ(data[3], 'C');
	EXPECT_EQ(data[4], '\0');
}

TEST(MyStringConstructorsTest, BufferAndLengthConstructor_ZeroLength)
{
	const MyString str("Test", 0);
	EXPECT_EQ(str.GetLength(), 0);
	EXPECT_STREQ(str.GetStringData(), "");
}

TEST(MyStringConstructorsTest, CopyConstructor)
{
	const MyString original("Copy me");
	const MyString copy(original);

	EXPECT_EQ(copy.GetLength(), original.GetLength());
	EXPECT_STREQ(copy.GetStringData(), original.GetStringData());
	EXPECT_NE(copy.GetStringData(), original.GetStringData());
}

TEST(MyStringConstructorsTest, CopyConstructor_EmbeddedNulls)
{
	constexpr char buf[] = { 'X', '\0', 'Y' };
	const MyString original(buf, 3);
	const MyString copy(original);

	EXPECT_EQ(copy.GetLength(), 3);
	EXPECT_EQ(copy.GetStringData()[1], '\0');
	EXPECT_EQ(copy.GetStringData()[2], 'Y');
}

TEST(MyStringConstructorsTest, MoveConstructor)
{
	MyString original("Move me");

	const MyString moved(std::move(original));

	EXPECT_EQ(moved.GetLength(), 7);
	EXPECT_STREQ(moved.GetStringData(), "Move me");

	EXPECT_EQ(original.GetLength(), 0);
	EXPECT_EQ(original.GetStringData()[0], '\0');
}

TEST(MyStringConstructorsTest, StdStringConstructor_Normal)
{
	const std::string stlStr = "STL String";
	const MyString str(stlStr);
	EXPECT_EQ(str.GetLength(), 10);
	EXPECT_STREQ(str.GetStringData(), "STL String");
}

TEST(MyStringConstructorsTest, StdStringConstructor_WithEmbeddedNulls)
{
	const std::string stlStr("A\0B", 3);
	const MyString str(stlStr);
	EXPECT_EQ(str.GetLength(), 3);
	EXPECT_EQ(str.GetStringData()[0], 'A');
	EXPECT_EQ(str.GetStringData()[1], '\0');
	EXPECT_EQ(str.GetStringData()[2], 'B');
	EXPECT_EQ(str.GetStringData()[3], '\0');
}

TEST(MyStringBasicStateTest, ClearMethod)
{
	MyString str("To be cleared");
	EXPECT_EQ(str.GetLength(), 13);

	str.Clear();
	EXPECT_EQ(str.GetLength(), 0);
	EXPECT_STREQ(str.GetStringData(), "");
	EXPECT_EQ(str.GetStringData()[0], '\0');
}

TEST(MyStringBasicStateTest, ClearMethod_AlreadyEmpty)
{
	MyString str;
	str.Clear();
	EXPECT_EQ(str.GetLength(), 0);
	EXPECT_STREQ(str.GetStringData(), "");
}