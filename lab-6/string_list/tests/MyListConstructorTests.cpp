#include "../MyList.hpp"
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(MyListConstructorTests, DefaultConstructor)
{
	MyList<int> intList;
	EXPECT_TRUE(intList.Empty());
	EXPECT_EQ(intList.Size(), 0);

	MyList<std::string> stringList;
	EXPECT_TRUE(stringList.Empty());
	EXPECT_EQ(stringList.Size(), 0);
}

TEST(MyListConstructorTests, CopyConstructor)
{
	MyList<int> originalInt;
	originalInt.PushBack(10);
	originalInt.PushBack(20);

	MyList<int> copyInt(originalInt);
	EXPECT_EQ(copyInt.Size(), 2);
	EXPECT_EQ(copyInt.Front(), 10);
	EXPECT_EQ(copyInt.Back(), 20);

	MyList<std::string> originalStr;
	originalStr.PushBack("hello");
	originalStr.PushBack("world");

	MyList<std::string> copyStr(originalStr);
	EXPECT_EQ(copyStr.Size(), 2);
	EXPECT_EQ(copyStr.Front(), "hello");
	EXPECT_EQ(copyStr.Back(), "world");

	copyStr.Front() = "modified";
	EXPECT_EQ(originalStr.Front(), "hello");
}

TEST(MyListConstructorTests, MoveConstructor)
{
	MyList<int> sourceInt;
	sourceInt.PushBack(1);
	MyList<int> destInt(std::move(sourceInt));

	EXPECT_EQ(destInt.Size(), 1);
	EXPECT_EQ(destInt.Front(), 1);
	EXPECT_TRUE(sourceInt.Empty());

	MyList<std::string> sourceStr;
	sourceStr.PushBack("move_me");
	MyList<std::string> destStr(std::move(sourceStr));

	EXPECT_EQ(destStr.Size(), 1);
	EXPECT_EQ(destStr.Front(), "move_me");
	EXPECT_TRUE(sourceStr.Empty());
}

TEST(MyListConstructorTests, CopyAssignment)
{
	MyList<int> list1;
	list1.PushBack(100);
	MyList<int> list2;
	list2.PushBack(200);

	list1 = list2;
	EXPECT_EQ(list1.Size(), 1);
	EXPECT_EQ(list1.Front(), 200);

	MyList<std::string> s1;
	s1.PushBack("A");
	MyList<std::string> s2;
	s2.PushBack("B");

	s1 = s2;
	EXPECT_EQ(s1.Front(), "B");

	s1 = *&s1;
	EXPECT_EQ(s1.Front(), "B");
}

TEST(MyListConstructorTests, MoveAssignment)
{
	MyList<int> list1;
	MyList<int> list2;
	list2.PushBack(500);

	list1 = std::move(list2);
	EXPECT_EQ(list1.Size(), 1);
	EXPECT_EQ(list1.Front(), 500);
	EXPECT_TRUE(list2.Empty());

	MyList<std::string> s1;
	MyList<std::string> s2;
	s2.PushBack("temporary");

	s1 = std::move(s2);
	EXPECT_EQ(s1.Front(), "temporary");
}