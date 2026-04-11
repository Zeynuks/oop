#include "../MyList.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <string>

TEST(MyListAdditionalTypesTests, WorkWithUniquePtr)
{
	MyList<std::unique_ptr<int>> list;
	list.PushBack(std::make_unique<int>(10));
	list.PushBack(std::make_unique<int>(20));

	EXPECT_EQ(*list.Front(), 10);
	EXPECT_EQ(*list.Back(), 20);

	auto moved_list = std::move(list);
	EXPECT_EQ(moved_list.Size(), 2);
	EXPECT_TRUE(list.Empty());
	EXPECT_EQ(*moved_list.Front(), 10);
}

TEST(MyListAdditionalTypesTests, WorkWithStructs)
{
	struct Point
	{
		int x, y;
		bool operator==(const Point&) const = default;
	};

	MyList<Point> list;
	list.PushBack({ 1, 2 });
	list.PushFront({ 0, 0 });

	EXPECT_EQ(list.Front().x, 0);
	EXPECT_EQ(list.Back().y, 2);
}

TEST(MyListAdditionalTypesTests, ListOfLists)
{
	MyList<MyList<int>> nestedList;

	MyList<int> inner;
	inner.PushBack(1);
	inner.PushBack(2);

	nestedList.PushBack(inner);
	nestedList.PushBack(std::move(inner));

	EXPECT_EQ(nestedList.Size(), 2);
	EXPECT_EQ(nestedList.Front().Size(), 2);
	EXPECT_EQ(nestedList.Back().Size(), 2);
}

TEST(MyListAdditionalTypesTests, LargeStringObjects)
{
	MyList<std::string> list;
	std::string large(1000, 'a');

	list.PushBack(large);
	list.PushBack(std::move(large));

	EXPECT_EQ(list.Size(), 2);
	EXPECT_EQ(list.Front().length(), 1000);
	EXPECT_EQ(list.Back().length(), 1000);
}

TEST(MyListAdditionalTypesTests, ConstCorrectness)
{
	MyList<int> list;
	list.PushBack(1);

	const auto& clist = list;
	auto it = clist.begin();

	static_assert(std::is_const_v<std::remove_reference_t<decltype(*it)>>);
	EXPECT_EQ(*it, 1);
}