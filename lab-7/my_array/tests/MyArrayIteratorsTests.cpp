#include "../MyArray.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <numeric>
#include <string>

TEST(MyArrayIterators, BeginEndForward)
{
	MyArray arr = { 1, 2, 3, 4, 5 };
	int sum = 0;
	for (auto it = arr.begin(); it != arr.end(); ++it)
	{
		sum += *it;
	}
	EXPECT_EQ(sum, 15);
}

TEST(MyArrayIterators, BeginEndModify)
{
	MyArray<std::string> arr = { "a", "b" };
	for (auto it = arr.begin(); it != arr.end(); ++it)
	{
		*it += "!";
	}
	EXPECT_EQ(arr[0], "a!");
	EXPECT_EQ(arr[1], "b!");
}

TEST(MyArrayIterators, ConstBeginEnd)
{
	const MyArray arr = { 1.1f, 2.2f };
	float sum = 0.0f;
	for (auto it = arr.begin(); it != arr.end(); ++it)
	{
		sum += *it;
	}
	EXPECT_FLOAT_EQ(sum, 3.3f);
}

TEST(MyArrayIterators, CBeginCEnd)
{
	const MyArray arr = { 10, 20 };
	const auto it = arr.cbegin();
	EXPECT_EQ(*it, 10);
	EXPECT_EQ(*(arr.cend() - 1), 20);
}

TEST(MyArrayIterators, RBeginREnd)
{
	MyArray<std::string> arr = { "last", "middle", "first" };
	auto it = arr.rbegin();
	EXPECT_EQ(*it, "first");
	++it;
	EXPECT_EQ(*it, "middle");
	++it;
	EXPECT_EQ(*it, "last");
	++it;
	EXPECT_EQ(it, arr.rend());
}

TEST(MyArrayIterators, RBeginREndModify)
{
	MyArray arr = { 1, 2 };
	*arr.rbegin() = 10;
	EXPECT_EQ(arr[1], 10);
}

TEST(MyArrayIterators, CRBeginCREnd)
{
	const MyArray arr = { 1.0f, 2.0f };
	const auto it = arr.crbegin();
	EXPECT_FLOAT_EQ(*it, 2.0f);
	EXPECT_FLOAT_EQ(*(arr.crend() - 1), 1.0f);
}

TEST(MyArrayIterators, EmptyArrayIterators)
{
	MyArray<int> arr;
	EXPECT_EQ(arr.begin(), arr.end());
	EXPECT_EQ(arr.rbegin(), arr.rend());
	EXPECT_EQ(arr.cbegin(), arr.cend());
}

TEST(MyArrayIterators, STLCompatibility)
{
	MyArray arr = { 3, 1, 4, 2 };
	std::sort(arr.begin(), arr.end());
	EXPECT_EQ(arr[0], 1);
	EXPECT_EQ(arr[3], 4);
}