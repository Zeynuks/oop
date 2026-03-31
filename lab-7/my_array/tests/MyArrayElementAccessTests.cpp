#include "../MyArray.hpp"

#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

TEST(MyArrayElementAccess, OperatorIndex)
{
	MyArray arr = { 1.1f, 2.2f, 3.3f };
	EXPECT_FLOAT_EQ(arr[0], 1.1f);
	EXPECT_FLOAT_EQ(arr[1], 2.2f);
	arr[1] = 5.5f;
	EXPECT_FLOAT_EQ(arr[1], 5.5f);
}

TEST(MyArrayElementAccess, ConstOperatorIndex)
{
	const MyArray<std::string> arr = { "A", "B" };
	EXPECT_EQ(arr[0], "A");
	EXPECT_EQ(arr[1], "B");
}

TEST(MyArrayElementAccess, AtMethod)
{
	MyArray arr = { 10, 20 };
	EXPECT_EQ(arr.At(0), 10);
	arr.At(1) = 30;
	EXPECT_EQ(arr.At(1), 30);
}

TEST(MyArrayElementAccess, AtConstMethod)
{
	const MyArray arr = { 1.5f };
	EXPECT_FLOAT_EQ(arr.At(0), 1.5f);
}

TEST(MyArrayElementAccess, AtOutOfBounds)
{
	MyArray arr = { 1, 2 };
	EXPECT_THROW(arr.At(2), std::out_of_range);
	EXPECT_THROW(arr.At(100), std::out_of_range);
}

TEST(MyArrayElementAccess, AtOutOfBoundsEmpty)
{
	MyArray<std::string> arr;
	EXPECT_THROW(arr.At(0), std::out_of_range);
}

TEST(MyArrayElementAccess, Front)
{
	MyArray<std::string> arr = { "first", "last" };
	EXPECT_EQ(arr.Front(), "first");
	arr.Front() = "new_first";
	EXPECT_EQ(arr[0], "new_first");
}

TEST(MyArrayElementAccess, FrontConst)
{
	const MyArray arr = { 9.9f };
	EXPECT_FLOAT_EQ(arr.Front(), 9.9f);
}

TEST(MyArrayElementAccess, Back)
{
	MyArray arr = { 1, 2, 3 };
	EXPECT_EQ(arr.Back(), 3);
	arr.Back() = 10;
	EXPECT_EQ(arr[2], 10);
}

TEST(MyArrayElementAccess, BackConst)
{
	const MyArray<std::string> arr = { "A", "B", "C" };
	EXPECT_EQ(arr.Back(), "C");
}