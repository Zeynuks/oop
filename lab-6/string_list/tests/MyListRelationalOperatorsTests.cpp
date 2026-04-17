#include "../MyList.hpp"
#include <compare>
#include <gtest/gtest.h>
#include <string>

TEST(MyListRelationalOperatorsTests, EqualityEmptyLists)
{
	MyList<int> list1;
	MyList<int> list2;
	EXPECT_TRUE(list1 == list2);
	EXPECT_FALSE(list1 != list2);
}

TEST(MyListRelationalOperatorsTests, EqualityDifferentSizes)
{
	MyList<int> list1;
	list1.PushBack(1);

	MyList<int> list2;
	list2.PushBack(1);
	list2.PushBack(2);

	EXPECT_FALSE(list1 == list2);
	EXPECT_TRUE(list1 != list2);
}

TEST(MyListRelationalOperatorsTests, EqualityDifferentValues)
{
	MyList<std::string> list1;
	list1.PushBack("A");

	MyList<std::string> list2;
	list2.PushBack("B");

	EXPECT_FALSE(list1 == list2);
}

TEST(MyListRelationalOperatorsTests, ThreeWayComparisonSame)
{
	MyList<int> list1;
	list1.PushBack(1);
	list1.PushBack(2);

	MyList<int> list2;
	list2.PushBack(1);
	list2.PushBack(2);

	EXPECT_EQ(list1 <=> list2, std::strong_ordering::equal);
	EXPECT_TRUE(list1 <= list2);
	EXPECT_TRUE(list1 >= list2);
}

TEST(MyListRelationalOperatorsTests, ThreeWayComparisonLess)
{
	MyList<int> list1;
	list1.PushBack(1);
	list1.PushBack(2);

	MyList<int> list2;
	list2.PushBack(1);
	list2.PushBack(3);

	EXPECT_EQ(list1 <=> list2, std::strong_ordering::less);
	EXPECT_TRUE(list1 < list2);
}

TEST(MyListRelationalOperatorsTests, ThreeWayComparisonGreater)
{
	MyList<std::string> list1;
	list1.PushBack("zeta");

	MyList<std::string> list2;
	list2.PushBack("alpha");

	EXPECT_EQ(list1 <=> list2, std::strong_ordering::greater);
	EXPECT_TRUE(list1 > list2);
}

TEST(MyListRelationalOperatorsTests, LexicographicalShortPrefix)
{
	MyList<int> list1;
	list1.PushBack(1);
	list1.PushBack(2);

	MyList<int> list2;
	list2.PushBack(1);
	list2.PushBack(2);
	list2.PushBack(3);

	EXPECT_TRUE(list1 < list2);
}