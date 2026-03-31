#include "../MyArray.hpp"

#include <gtest/gtest.h>
#include <string>

TEST(MyArrayComparison, EqualityOperatorInt)
{
	const MyArray arr1 = { 1, 2, 3 };
	const MyArray arr2 = { 1, 2, 3 };
	const MyArray arr3 = { 1, 2, 4 };
	const MyArray arr4 = { 1, 2 };

	EXPECT_TRUE(arr1 == arr2);
	EXPECT_FALSE(arr1 == arr3);
	EXPECT_FALSE(arr1 == arr4);
}

TEST(MyArrayComparison, EqualityOperatorString)
{
	const MyArray<std::string> arr1 = { "hello", "world" };
	const MyArray<std::string> arr2 = { "hello", "world" };
	const MyArray<std::string> arr3 = { "hello", "cpp" };

	EXPECT_TRUE(arr1 == arr2);
	EXPECT_FALSE(arr1 == arr3);
}

TEST(MyArrayComparison, ThreeWayComparisonInt)
{
	const MyArray arr1 = { 1, 2, 3 };
	const MyArray arr2 = { 1, 2, 3 };
	const MyArray arr3 = { 1, 2, 4 };
	const MyArray arr4 = { 1, 2 };

	EXPECT_TRUE((arr1 <=> arr2) == 0);
	EXPECT_TRUE((arr1 <=> arr3) < 0);
	EXPECT_TRUE((arr3 <=> arr1) > 0);
	EXPECT_TRUE((arr4 <=> arr1) < 0);
}

TEST(MyArrayComparison, ThreeWayComparisonFloat)
{
	const MyArray arr1 = { 1.1f, 2.2f };
	const MyArray arr2 = { 1.1f, 2.3f };

	EXPECT_TRUE(arr1 < arr2);
	EXPECT_TRUE(arr2 > arr1);
	EXPECT_TRUE(arr1 <= arr2);
	EXPECT_TRUE(arr1 != arr2);
}

TEST(MyArrayComparison, EmptyArraysEquality)
{
	constexpr MyArray<int> arr1;
	constexpr MyArray<int> arr2;

	EXPECT_TRUE(arr1 == arr2);
	EXPECT_TRUE((arr1 <=> arr2) == 0);
}

TEST(MyArrayComparison, LexicographicalString)
{
	const MyArray<std::string> arr1 = { "apple", "banana" };
	const MyArray<std::string> arr2 = { "apple", "cherry" };

	EXPECT_TRUE(arr1 < arr2);
	EXPECT_FALSE(arr1 > arr2);
}