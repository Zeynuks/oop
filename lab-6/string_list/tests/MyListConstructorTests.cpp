#include "../MyList.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

TEST(MyListTest, DefaultConstructor) {
	MyList<std::string> list;
	EXPECT_EQ(list.Size(), 0);
	EXPECT_TRUE(list.Empty());
}

TEST(MyListTest, CopyConstructor) {
	MyList<std::string> original;
	original.PushBack("first");
	original.PushBack("second");

	MyList<std::string> copy(original);
	EXPECT_EQ(copy.Size(), 2);
	EXPECT_EQ(copy.Front(), "first");
	EXPECT_EQ(copy.Back(), "second");

	copy.Front() = "modified";
	EXPECT_EQ(original.Front(), "first");
}

TEST(MyListTest, MoveConstructor) {
	MyList<std::string> original;
	original.PushBack("move_me");

	MyList<std::string> moved(std::move(original));

	EXPECT_EQ(moved.Size(), 1);
	EXPECT_EQ(moved.Front(), "move_me");
	EXPECT_EQ(original.Size(), 0);
	EXPECT_TRUE(original.Empty());
}

TEST(MyListTest, AllocatorConstructor) {
	std::allocator<std::string> alloc;
	MyList<std::string> list(alloc);
	EXPECT_TRUE(list.Empty());
}

TEST(MyListTest, EmptyStateInvariants) {
	MyList<int> list;
	EXPECT_EQ(list.begin(), list.end());
	EXPECT_EQ(list.Size(), 0);
}