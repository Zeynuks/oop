#include "../MyArray.hpp"

#include <gtest/gtest.h>
#include <string>

TEST(MyArrayConstructors, DefaultConstructor)
{
	constexpr MyArray<int> arr;

	EXPECT_EQ(arr.Size(), 0);
	EXPECT_TRUE(arr.Empty());
}

TEST(MyArrayConstructors, InitializerListConstructorString)
{
	MyArray<std::string> arr = { "one", "two", "three" };

	EXPECT_EQ(arr.Size(), 3);
	EXPECT_EQ(arr[0], "one");
	EXPECT_EQ(arr[2], "three");
}

TEST(MyArrayConstructors, InitializerListConstructorFloat)
{
	MyArray arr = { 1.1f, 2.2f };

	EXPECT_EQ(arr.Size(), 2);
	EXPECT_FLOAT_EQ(arr[0], 1.1f);
}

TEST(MyArrayConstructors, CopyConstructor)
{
	MyArray<std::string> original = { "hello", "world" };
	MyArray copy(original);

	EXPECT_EQ(copy.Size(), original.Size());
	EXPECT_EQ(copy[0], "hello");
	copy[0] = "bye";
	EXPECT_EQ(original[0], "hello");
}

TEST(MyArrayConstructors, MoveConstructor)
{
	MyArray<std::string> original = { "data" };
	MyArray moved(std::move(original));

	EXPECT_EQ(moved.Size(), 1);
	EXPECT_EQ(moved[0], "data");
	EXPECT_TRUE(original.Empty());
}

TEST(MyArrayAssignment, CopyAssignment)
{
	const MyArray arr1 = { 1.0f, 2.0f };
	MyArray<float> arr2 = arr1;

	EXPECT_EQ(arr2.Size(), 2);
	EXPECT_FLOAT_EQ(arr2[1], 2.0f);
}

TEST(MyArrayAssignment, SelfCopyAssignment)
{
	MyArray<std::string> arr = { "test" };
	arr = arr;
	EXPECT_EQ(arr.Size(), 1);
	EXPECT_EQ(arr[0], "test");
}

TEST(MyArrayAssignment, MoveAssignment)
{
	MyArray<std::string> arr1 = { "a", "b" };
	MyArray<std::string> arr2 = std::move(arr1);
	EXPECT_EQ(arr2.Size(), 2);
	EXPECT_EQ(arr2[0], "a");
	EXPECT_TRUE(arr1.Empty());
}

TEST(MyArrayAssignment, SelfMoveAssignment)
{
	MyArray<std::string> arr = { "test" };
	arr = std::move(arr);
	EXPECT_EQ(arr.Size(), 1);
	EXPECT_EQ(arr[0], "test");
}

TEST(MyArrayAssignment, InitializerListAssignment)
{
	MyArray<int> arr = { 10, 20, 30 };
	EXPECT_EQ(arr.Size(), 3);
	EXPECT_EQ(arr[2], 30);
}

TEST(MyArrayString, DeepCopy)
{
	MyArray<std::string> arr1 = { "alpha", "beta" };
	MyArray<std::string> arr2 = arr1;

	arr2[0] = "changed";

	EXPECT_EQ(arr1[0], "alpha");
	EXPECT_EQ(arr2[0], "changed");
}

TEST(MyArrayString, MoveStrings)
{
	MyArray<std::string> arr1 = { "first", "second" };
	MyArray<std::string> arr2 = std::move(arr1);

	EXPECT_EQ(arr2.Size(), 2);
	EXPECT_EQ(arr2[0], "first");
	EXPECT_EQ(arr2[1], "second");
	EXPECT_TRUE(arr1.Empty());
}

TEST(MyArrayString, LargeStrings)
{
	std::string big1(1000, 'a');
	std::string big2(2000, 'b');

	MyArray<std::string> arr = { big1, big2 };

	EXPECT_EQ(arr.Size(), 2);
	EXPECT_EQ(arr[0], big1);
	EXPECT_EQ(arr[1], big2);
}

TEST(MyArrayString, ModifyAfterCopy)
{
	MyArray<std::string> arr1 = { "one", "two" };
	MyArray<std::string> arr2 = arr1;

	arr1[1] = "changed";

	EXPECT_EQ(arr1[1], "changed");
	EXPECT_EQ(arr2[1], "two");
}

TEST(MyArrayString, MoveAssignmentStrings)
{
	MyArray<std::string> arr1 = { "hello", "world" };
	MyArray<std::string> arr2;

	arr2 = std::move(arr1);

	EXPECT_EQ(arr2.Size(), 2);
	EXPECT_EQ(arr2[0], "hello");
	EXPECT_TRUE(arr1.Empty());
}