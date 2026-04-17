#include "../MyList.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(MyListIteratorTests, ForwardIteration)
{
	MyList<int> intList;
	intList.PushBack(1);
	intList.PushBack(2);
	intList.PushBack(3);

	std::vector<int> result;
	for (auto it = intList.begin(); it != intList.end(); ++it)
	{
		result.push_back(*it);
	}
	EXPECT_EQ(result, (std::vector<int>{ 1, 2, 3 }));

	MyList<std::string> stringList;
	stringList.PushBack("a");
	stringList.PushBack("b");

	auto sIt = stringList.begin();
	EXPECT_EQ(*sIt, "a");
	EXPECT_EQ(*(++sIt), "b");
}

TEST(MyListIteratorTests, ConstIteration)
{
	MyList<int> intList;
	intList.PushBack(10);
	const MyList<int>& constList = intList;

	auto it = constList.begin();
	EXPECT_EQ(*it, 10);

	EXPECT_EQ(constList.cbegin(), constList.begin());
	EXPECT_EQ(constList.cend(), constList.end());
}

TEST(MyListIteratorTests, ReverseIteration)
{
	MyList<int> intList;
	intList.PushBack(1);
	intList.PushBack(2);
	intList.PushBack(3);

	std::vector<int> result;
	for (auto it = intList.rbegin(); it != intList.rend(); ++it)
	{
		result.push_back(*it);
	}
	EXPECT_EQ(result, (std::vector<int>{ 3, 2, 1 }));

	MyList<std::string> stringList;
	stringList.PushBack("first");
	stringList.PushBack("last");

	EXPECT_EQ(*stringList.rbegin(), "last");
	EXPECT_EQ(*(--stringList.rend()), "first");
}

TEST(MyListIteratorTests, IteratorOperators)
{
	MyList<int> list;
	list.PushBack(1);
	list.PushBack(2);

	auto it1 = list.begin();
	auto it2 = list.begin();

	EXPECT_TRUE(it1 == it2);
	EXPECT_FALSE(it1 != it2);

	it2++;
	EXPECT_TRUE(it1 != it2);

	auto it3 = it2--;
	EXPECT_EQ(*it3, 2);
	EXPECT_EQ(*it2, 1);

	auto it4 = ++it1;
	EXPECT_EQ(*it4, 2);
	EXPECT_EQ(*it1, 2);
}

TEST(MyListIteratorTests, IteratorConversion)
{
	MyList<int> list;
	list.PushBack(1);

	MyList<int>::iterator it = list.begin();
	MyList<int>::const_iterator cit = it;

	EXPECT_EQ(*it, *cit);
	EXPECT_TRUE(it == cit);
}

TEST(MyListIteratorTests, ArrowOperator)
{
	struct Mock
	{
		int x;
	};
	MyList<Mock> list;
	list.PushBack({ 42 });

	EXPECT_EQ(list.begin()->x, 42);
}

TEST(MyListIteratorTests, STLCompatibility)
{
	MyList<int> list;
	list.PushBack(3);
	list.PushBack(1);
	list.PushBack(2);

	auto it = std::find(list.begin(), list.end(), 1);
	EXPECT_NE(it, list.end());
	EXPECT_EQ(*it, 1);
}