#include "../MyArray.hpp"

#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(MyArrayModifiers, PushBackCopy)
{
	MyArray<std::string> arr;
	const std::string val = "test";
	arr.PushBack(val);
	EXPECT_EQ(arr.Size(), 1);
	EXPECT_EQ(arr[0], "test");
	EXPECT_EQ(val, "test");
}

TEST(MyArrayModifiers, PushBackMove)
{
	MyArray<std::string> arr;
	std::string val = "move_me";
	arr.PushBack(std::move(val));
	EXPECT_EQ(arr.Size(), 1);
	EXPECT_EQ(arr[0], "move_me");
	EXPECT_TRUE(val.empty());
}

TEST(MyArrayModifiers, PushBackCapacityDoubling)
{
	MyArray<int> arr;
	arr.PushBack(1);
	EXPECT_EQ(arr.Capacity(), 1);
	arr.PushBack(2);
	EXPECT_EQ(arr.Capacity(), 2);
	arr.PushBack(3);
	EXPECT_EQ(arr.Capacity(), 4);
	EXPECT_EQ(arr.Size(), 3);
}

TEST(MyArrayModifiers, PopBack)
{
	MyArray arr = { 1.1f, 2.2f };
	arr.PopBack();
	EXPECT_EQ(arr.Size(), 1);
	EXPECT_FLOAT_EQ(arr.Back(), 1.1f);
	arr.PopBack();
	EXPECT_TRUE(arr.Empty());
}

TEST(MyArrayModifiers, PopBackEmpty)
{
	MyArray<int> arr;
	arr.PopBack();
	EXPECT_TRUE(arr.Empty());
}

TEST(MyArrayModifiers, EmplaceBack)
{
	MyArray<std::string> arr;
	arr.EmplaceBack(3, 's');
	EXPECT_EQ(arr.Size(), 1);
	EXPECT_EQ(arr[0], "sss");
}

TEST(MyArrayModifiers, InsertSingle)
{
	MyArray arr = { 1, 3 };
	const auto it = arr.Insert(arr.begin() + 1, 2);
	EXPECT_EQ(*it, 2);
	EXPECT_EQ(arr.Size(), 3);
	EXPECT_EQ(arr[0], 1);
	EXPECT_EQ(arr[1], 2);
	EXPECT_EQ(arr[2], 3);
}

TEST(MyArrayModifiers, InsertAtEnd)
{
	MyArray<std::string> arr = { "A" };
	arr.Insert(arr.end(), "B");
	EXPECT_EQ(arr.Size(), 2);
	EXPECT_EQ(arr[1], "B");
}

TEST(MyArrayModifiers, EraseSingle)
{
	MyArray arr = { 1.0f, 2.0f, 3.0f };
	const auto it = arr.Erase(arr.begin() + 1);
	EXPECT_FLOAT_EQ(*it, 3.0f);
	EXPECT_EQ(arr.Size(), 2);
	EXPECT_FLOAT_EQ(arr[0], 1.0f);
	EXPECT_FLOAT_EQ(arr[1], 3.0f);
}

TEST(MyArrayModifiers, EraseRange)
{
	MyArray arr = { 0, 1, 2, 3, 4 };
	const auto it = arr.Erase(arr.begin() + 1, arr.begin() + 4);
	EXPECT_EQ(*it, 4);
	EXPECT_EQ(arr.Size(), 2);
	EXPECT_EQ(arr[0], 0);
	EXPECT_EQ(arr[1], 4);
}

TEST(MyArrayModifiers, EraseRangeEmpty)
{
	MyArray arr = { 1, 2 };
	const auto it = arr.Erase(arr.begin(), arr.begin());
	EXPECT_EQ(*it, 1);
	EXPECT_EQ(arr.Size(), 2);
}

TEST(MyArrayModifiers, Swap)
{
	MyArray a = { 1, 2 };
	MyArray b = { 3, 4, 5 };
	a.Swap(b);
	EXPECT_EQ(a.Size(), 3);
	EXPECT_EQ(b.Size(), 2);
	EXPECT_EQ(a[0], 3);
	EXPECT_EQ(b[0], 1);
}

TEST(MyArrayModifiers, AssignCount)
{
	MyArray<std::string> arr;
	arr.Assign(2, "val");
	EXPECT_EQ(arr.Size(), 2);
	EXPECT_EQ(arr[0], "val");
	EXPECT_EQ(arr[1], "val");
}

TEST(MyArrayModifiers, AssignRange)
{
	std::vector vec = { 0.5f, 1.5f };
	MyArray<float> arr;
	arr.Assign(vec.begin(), vec.end());
	EXPECT_EQ(arr.Size(), 2);
	EXPECT_FLOAT_EQ(arr[0], 0.5f);
}