#include "../MyString.hpp"

#include <gtest/gtest.h>

TEST(MyStringAssignmentTest, CopyAssignment_Normal)
{
	const MyString str1("First");
    MyString str2("Second");
    
    str2 = str1;
    
    EXPECT_EQ(str2.GetLength(), 5);
    EXPECT_STREQ(str2.GetStringData(), "First");
    EXPECT_NE(str2.GetStringData(), str1.GetStringData());
}

TEST(MyStringAssignmentTest, CopyAssignment_SelfAssignment)
{
    MyString str("Self");
    str = str;
    
    EXPECT_EQ(str.GetLength(), 4);
    EXPECT_STREQ(str.GetStringData(), "Self");
}

TEST(MyStringAssignmentTest, CopyAssignment_WithEmbeddedNulls)
{
    const char buf[] = {'A', '\0', 'B'};
	const MyString str1(buf, 3);
    MyString str2;
    
    str2 = str1;
    
    EXPECT_EQ(str2.GetLength(), 3);
    EXPECT_EQ(str2.GetStringData()[1], '\0');
    EXPECT_EQ(str2.GetStringData()[2], 'B');
}

TEST(MyStringAssignmentTest, MoveAssignment_Normal)
{
    MyString str1("Movable");
    MyString str2("Target");
    
    str2 = std::move(str1);
    
    EXPECT_EQ(str2.GetLength(), 7);
    EXPECT_STREQ(str2.GetStringData(), "Movable");
    
    EXPECT_EQ(str1.GetLength(), 0);
    EXPECT_EQ(str1.GetStringData()[0], '\0');
}

TEST(MyStringAssignmentTest, MoveAssignment_SelfMove)
{
    MyString str("MoveMe");
    str = std::move(str);
    
    EXPECT_EQ(str.GetLength(), 6);
    EXPECT_STREQ(str.GetStringData(), "MoveMe");
}

TEST(MyStringSubStringTest, SubString_Middle)
{
	const MyString str("Hello World");
	const MyString sub = str.SubString(6, 5);
    
    EXPECT_EQ(sub.GetLength(), 5);
    EXPECT_STREQ(sub.GetStringData(), "World");
}

TEST(MyStringSubStringTest, SubString_ToTheEnd)
{
	const MyString str("Example");
	const MyString sub = str.SubString(2);
    
    EXPECT_EQ(sub.GetLength(), 5);
    EXPECT_STREQ(sub.GetStringData(), "ample");
}

TEST(MyStringSubStringTest, SubString_LengthExceedsAvailable)
{
	const MyString str("Short");
	const MyString sub = str.SubString(3, 100);
    
    EXPECT_EQ(sub.GetLength(), 2);
    EXPECT_STREQ(sub.GetStringData(), "rt");
}

TEST(MyStringSubStringTest, SubString_EmbeddedNulls)
{
    const char buf[] = {'A', '\0', 'B', 'C', '\0', 'D'};
	const MyString str(buf, 6);

	const MyString sub = str.SubString(0, 3);
    
    EXPECT_EQ(sub.GetLength(), 3);
    EXPECT_EQ(sub.GetStringData()[0], 'A');
    EXPECT_EQ(sub.GetStringData()[1], '\0');
    EXPECT_EQ(sub.GetStringData()[2], 'B');
    EXPECT_EQ(sub.GetStringData()[3], '\0');
}

TEST(MyStringSubStringTest, SubString_EmptyResult)
{
	const MyString str("Test");
	const MyString sub = str.SubString(2, 0);
    
    EXPECT_EQ(sub.GetLength(), 0);
    EXPECT_STREQ(sub.GetStringData(), "");
}

TEST(MyStringSubStringTest, SubString_OutOfRange_Throws)
{
	const MyString str("Index");
    EXPECT_THROW(str.SubString(10, 1), std::out_of_range);
}

TEST(MyStringSubStringTest, SubString_StartAtLength)
{
	const MyString str("End");
	const MyString sub = str.SubString(3);
    EXPECT_EQ(sub.GetLength(), 0);
    EXPECT_STREQ(sub.GetStringData(), "");
}