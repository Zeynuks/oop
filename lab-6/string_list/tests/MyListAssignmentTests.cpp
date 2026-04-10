#include "../MyList.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using namespace ::testing;

TEST(MyListAssignmentTest, CopyAssignment)
{
	MyList<std::string> list1;
	list1.PushBack("alpha");
	list1.PushBack("beta");

	MyList<std::string> list2;
	list2.PushBack("temp");

	list2 = list1;

	EXPECT_EQ(list2.Size(), 2);
	EXPECT_EQ(list2.Front(), "alpha");
	EXPECT_EQ(list2.Back(), "beta");

	list2.Front() = "gamma";
	EXPECT_EQ(list1.Front(), "alpha");
}

TEST(MyListAssignmentTest, SelfCopyAssignment)
{
	MyList<int> list;
	list.PushBack(1);
	list.PushBack(2);

	list = list;

	EXPECT_EQ(list.Size(), 2);
	EXPECT_EQ(list.Front(), 1);
	EXPECT_EQ(list.Back(), 2);
}

TEST(MyListAssignmentTest, MoveAssignment)
{
	MyList<std::string> list1;
	list1.PushBack("move_target");

	MyList<std::string> list2;
	list2.PushBack("old_data");

	list2 = std::move(list1);

	EXPECT_EQ(list2.Size(), 1);
	EXPECT_EQ(list2.Front(), "move_target");
	EXPECT_TRUE(list1.Empty());
}

TEST(MyListAssignmentTest, SelfMoveAssignment)
{
	MyList<int> list;
	list.PushBack(100);

	list = std::move(list);

	EXPECT_EQ(list.Size(), 1);
	EXPECT_EQ(list.Front(), 100);
}

TEST(MyListAssignmentTest, SwapMemberFunction)
{
	MyList<int> list1;
	list1.PushBack(1);
	list1.PushBack(2);

	MyList<int> list2;
	list2.PushBack(10);

	list1.Swap(list2);

	EXPECT_EQ(list1.Size(), 1);
	EXPECT_EQ(list1.Front(), 10);
	EXPECT_EQ(list2.Size(), 2);
	EXPECT_EQ(list2.Front(), 1);
}

TEST(MyListAssignmentTest, MatcherComparison)
{
	MyList<int> list1;
	list1.PushBack(1);
	list1.PushBack(2);
	list1.PushBack(3);

	MyList<int> list2;
	list2 = list1;

	std::vector<int> expected = { 1, 2, 3 };
	EXPECT_THAT(list2, ElementsAreArray(expected));
}