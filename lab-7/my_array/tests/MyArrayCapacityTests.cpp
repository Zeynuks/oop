#include "../MyArray.hpp"

#include <gtest/gtest.h>
#include <string>

TEST(MyArrayCapacity, SizeAndEmpty)
{
	MyArray<int> arr;

	EXPECT_TRUE(arr.Empty());
	EXPECT_EQ(arr.Size(), 0);
	arr.PushBack(1);
	EXPECT_FALSE(arr.Empty());
	EXPECT_EQ(arr.Size(), 1);
}

TEST(MyArrayCapacity, CapacityAndReserve)
{
	MyArray<float> arr;

	EXPECT_EQ(arr.Capacity(), 0);
	arr.Reserve(10);
	EXPECT_EQ(arr.Capacity(), 10);
	EXPECT_EQ(arr.Size(), 0);
	arr.Reserve(5);
	EXPECT_EQ(arr.Capacity(), 10);
}

TEST(MyArrayCapacity, ResizeExpandDefault)
{
	MyArray arr = { 1, 2 };
	arr.Resize(4);

	EXPECT_EQ(arr.Size(), 4);
	EXPECT_EQ(arr[0], 1);
	EXPECT_EQ(arr[1], 2);
	EXPECT_EQ(arr[2], 0);
	EXPECT_EQ(arr[3], 0);
}

TEST(MyArrayCapacity, ResizeExpandWithValue)
{
	MyArray<std::string> arr = { "A" };
	arr.Resize(3, "B");

	EXPECT_EQ(arr.Size(), 3);
	EXPECT_EQ(arr[0], "A");
	EXPECT_EQ(arr[1], "B");
	EXPECT_EQ(arr[2], "B");
}

TEST(MyArrayCapacity, ResizeShrink)
{
	MyArray<std::string> arr = { "1", "2", "3" };
	arr.Resize(1);

	EXPECT_EQ(arr.Size(), 1);
	EXPECT_EQ(arr[0], "1");
}

TEST(MyArrayCapacity, ResizeToZero)
{
	MyArray arr = { 1.1f, 2.2f };
	arr.Resize(0);

	EXPECT_EQ(arr.Size(), 0);
	EXPECT_TRUE(arr.Empty());
}

TEST(MyArrayCapacity, Clear)
{
	MyArray<std::string> arr = { "test1", "test2" };
	arr.Clear();

	EXPECT_EQ(arr.Size(), 0);
	EXPECT_TRUE(arr.Empty());
}

TEST(MyArrayCapacity, ShrinkToFit)
{
	MyArray<int> arr;
	arr.Reserve(100);
	arr.PushBack(1);
	arr.PushBack(2);

	EXPECT_EQ(arr.Capacity(), 100);
	arr.ShrinkToFit();
	EXPECT_EQ(arr.Capacity(), 2);
	EXPECT_EQ(arr.Size(), 2);
}

TEST(MyArrayCapacity, ShrinkToFitEmpty)
{
	MyArray<float> arr;
	arr.Reserve(50);
	arr.ShrinkToFit();

	EXPECT_EQ(arr.Capacity(), 0);
}