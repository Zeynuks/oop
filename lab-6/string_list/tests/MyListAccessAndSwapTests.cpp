#include "../MyList.hpp"
#include <gtest/gtest.h>
#include <string>

TEST(MyListAccessAndSwapTests, FrontAndBackAccess)
{
	MyList<int> intList;
	intList.PushBack(10);
	intList.PushBack(20);
	intList.PushBack(30);

	EXPECT_EQ(intList.Front(), 10);
	EXPECT_EQ(intList.Back(), 30);

	intList.Front() = 100;
	intList.Back() = 300;

	EXPECT_EQ(intList.Front(), 100);
	EXPECT_EQ(intList.Back(), 300);

	const MyList<int>& constList = intList;
	EXPECT_EQ(constList.Front(), 100);
	EXPECT_EQ(constList.Back(), 300);

	MyList<std::string> stringList;
	stringList.PushBack("start");
	stringList.PushBack("finish");
	EXPECT_EQ(stringList.Front(), "start");
	EXPECT_EQ(stringList.Back(), "finish");
}

TEST(MyListAccessAndSwapTests, SwapMethods)
{
	MyList<int> list1;
	list1.PushBack(1);
	list1.PushBack(2);

	MyList<int> list2;
	list2.PushBack(99);

	list1.Swap(list2);

	EXPECT_EQ(list1.Size(), 1);
	EXPECT_EQ(list1.Front(), 99);

	EXPECT_EQ(list2.Size(), 2);
	EXPECT_EQ(list2.Front(), 1);
	EXPECT_EQ(list2.Back(), 2);

	MyList<int> emptyList;
	list1.Swap(emptyList);

	EXPECT_TRUE(list1.Empty());
	EXPECT_EQ(emptyList.Size(), 1);
	EXPECT_EQ(emptyList.Front(), 99);
}

TEST(MyListAccessAndSwapTests, EqualityOperator)
{
	MyList<int> list1;
	list1.PushBack(1);
	list1.PushBack(2);

	MyList<int> list2;
	list2.PushBack(1);
	list2.PushBack(2);

	EXPECT_TRUE(list1 == list2);

	list2.PushBack(3);
	EXPECT_FALSE(list1 == list2);

	MyList<std::string> sList1, sList2;
	EXPECT_TRUE(sList1 == sList2);

	sList1.PushBack("test");
	sList2.PushBack("test");
	EXPECT_TRUE(sList1 == sList2);
}

TEST(MyListAccessAndSwapTests, SpaceshipOperator)
{
	MyList<int> list1;
	list1.PushBack(1);
	list1.PushBack(2);

	MyList<int> list2;
	list2.PushBack(1);
	list2.PushBack(3);

	MyList<int> list3;
	list3.PushBack(1);
	list3.PushBack(2);
	list3.PushBack(0);

	EXPECT_TRUE(list1 < list2);
	EXPECT_TRUE(list1 <= list2);
	EXPECT_TRUE(list2 > list1);
	EXPECT_TRUE(list2 >= list1);

	EXPECT_TRUE(list1 < list3);

	MyList<std::string> sList1;
	sList1.PushBack("apple");

	MyList<std::string> sList2;
	sList2.PushBack("banana");

	EXPECT_TRUE(sList1 < sList2);
}

TEST(MyListAccessAndSwapTests, SizeAndEmptyProperties)
{
	MyList<int> list;
	EXPECT_TRUE(list.Empty());
	EXPECT_EQ(list.Size(), 0);

	list.PushBack(1);
	EXPECT_FALSE(list.Empty());
	EXPECT_EQ(list.Size(), 1);

	list.PushFront(2);
	EXPECT_EQ(list.Size(), 2);

	list.Erase(list.begin());
	EXPECT_EQ(list.Size(), 1);

	list.Clear();
	EXPECT_TRUE(list.Empty());
	EXPECT_EQ(list.Size(), 0);
}