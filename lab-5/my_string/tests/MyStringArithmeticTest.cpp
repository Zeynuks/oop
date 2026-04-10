#include "../MyString.hpp"

#include <gtest/gtest.h>

TEST(MyStringArithmeticTest, OperatorPlusAssign_MyString)
{
    MyString str1("Hello");
	const MyString str2(" World");
    str1 += str2;
    EXPECT_EQ(str1.GetLength(), 11);
    EXPECT_STREQ(str1.GetStringData(), "Hello World");
}

TEST(MyStringArithmeticTest, OperatorPlusAssign_StdString)
{
    MyString str("Base");
	const std::string add = "Extension";
    str += add;
    EXPECT_EQ(str.GetLength(), 13);
    EXPECT_STREQ(str.GetStringData(), "BaseExtension");
}

TEST(MyStringArithmeticTest, OperatorPlusAssign_CString)
{
    MyString str("C-");
    str += "String";
    EXPECT_EQ(str.GetLength(), 8);
    EXPECT_STREQ(str.GetStringData(), "C-String");
}

TEST(MyStringArithmeticTest, OperatorPlusAssign_EmbeddedNulls)
{
    char buf1[] = {'A', '\0', 'B'};
    char buf2[] = {'C', '\0', 'D'};
    MyString str1(buf1, 3);
	const MyString str2(buf2, 3);
    
    str1 += str2;
    
    EXPECT_EQ(str1.GetLength(), 6);
    const char* data = str1.GetStringData();
    EXPECT_EQ(data[0], 'A');
    EXPECT_EQ(data[1], '\0');
    EXPECT_EQ(data[2], 'B');
    EXPECT_EQ(data[3], 'C');
    EXPECT_EQ(data[4], '\0');
    EXPECT_EQ(data[5], 'D');
    EXPECT_EQ(data[6], '\0');
}

TEST(MyStringArithmeticTest, OperatorPlus_MyString_MyString)
{
	const MyString s1("abc");
	const MyString s2("def");
	const MyString res = s1 + s2;
    EXPECT_STREQ(res.GetStringData(), "abcdef");
}

TEST(MyStringArithmeticTest, OperatorPlus_MyString_StdString)
{
	const MyString s1("abc");
	const std::string s2 = "def";
	const MyString res = s1 + s2;
    EXPECT_STREQ(res.GetStringData(), "abcdef");
}

TEST(MyStringArithmeticTest, OperatorPlus_StdString_MyString)
{
	const std::string s1 = "abc";
	const MyString s2("def");
	const MyString res = s1 + s2;
    EXPECT_STREQ(res.GetStringData(), "abcdef");
}

TEST(MyStringArithmeticTest, OperatorPlus_MyString_CString)
{
	const MyString s1("abc");
	const MyString res = s1 + "def";
    EXPECT_STREQ(res.GetStringData(), "abcdef");
}

TEST(MyStringArithmeticTest, OperatorPlus_CString_MyString)
{
	const MyString s2("def");
	const MyString res = "abc" + s2;
    EXPECT_STREQ(res.GetStringData(), "abcdef");
}

TEST(MyStringArithmeticTest, CapacityDoublingRequirement)
{
    MyString str("1234567890");
	const size_t initialCapacity = str.GetCapacity();
    
    while(str.GetLength() < initialCapacity)
    {
        str += "a";
    }

	const size_t capBeforePush = str.GetCapacity();
    str += "b"; 
    
    EXPECT_GE(str.GetCapacity(), capBeforePush * 2);
}

TEST(MyStringArithmeticTest, PlusAssign_SelfConcatenation)
{
    MyString str("Abc");
    str += str;
    EXPECT_EQ(str.GetLength(), 6);
    EXPECT_STREQ(str.GetStringData(), "AbcAbc");
}