#include "../numbers.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(ReadNumbersTest, NormalInput)
{
	const std::istringstream input("1.5 2.5 3.0");
	std::cin.rdbuf(input.rdbuf());

	const Numbers numbers = ReadNumbers();
	ASSERT_EQ(numbers.size(), 3);
	EXPECT_DOUBLE_EQ(numbers[0], 1.5);
	EXPECT_DOUBLE_EQ(numbers[1], 2.5);
	EXPECT_DOUBLE_EQ(numbers[2], 3.0);
}

TEST(ReadNumbersTest, InvalidInput)
{
	const std::istringstream input("1.0 x 2.0");
	std::cin.rdbuf(input.rdbuf());

	EXPECT_THROW(ReadNumbers(), std::invalid_argument);
}

TEST(ProcessNumbersTest, PositiveNumbers)
{
	Numbers numbers = { 1.0, 2.0, 3.0 };
	ProcessNumbers(numbers);
	EXPECT_DOUBLE_EQ(numbers[0], 3.0);
	EXPECT_DOUBLE_EQ(numbers[1], 4.0);
	EXPECT_DOUBLE_EQ(numbers[2], 5.0);
}

TEST(ProcessNumbersTest, SomeNegativeNumbers)
{
	Numbers numbers = { -1.0, 2.0, -3.0 };
	ProcessNumbers(numbers);
	EXPECT_DOUBLE_EQ(numbers[0], 1.0);
	EXPECT_DOUBLE_EQ(numbers[1], 4.0);
	EXPECT_DOUBLE_EQ(numbers[2], -1.0);
}

TEST(ProcessNumbersTest, AllNegativeNumbers)
{
	Numbers numbers = { -1.0, -2.0 };
	ProcessNumbers(numbers);
	EXPECT_DOUBLE_EQ(numbers[0], -1.0);
	EXPECT_DOUBLE_EQ(numbers[1], -2.0);
}

TEST(ReadNumbersTest, EmptyInput)
{
	const std::istringstream input("");
	std::cin.rdbuf(input.rdbuf());

	const Numbers numbers = ReadNumbers();
	EXPECT_TRUE(numbers.empty());
}

TEST(ProcessNumbersTest, EmptyVector)
{
	Numbers numbers;
	ProcessNumbers(numbers);
	EXPECT_TRUE(numbers.empty());
}

TEST(PrintSortedNumbersTest, SortedOutput)
{
	const Numbers numbers = { 3.0, 1.0, 2.0 };
	const std::ostringstream out;
	std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());

	PrintSortedNumbers(numbers);

	std::cout.rdbuf(oldCout);
	const std::string expected = "\n1.000 2.000 3.000\n";
	EXPECT_EQ(out.str(), expected);
}

TEST(PrintSortedNumbersTest, EmptyVector)
{
	constexpr Numbers numbers;
	const std::ostringstream out;
	std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());

	PrintSortedNumbers(numbers);

	std::cout.rdbuf(oldCout);
	const std::string expected = "\n\n";
	EXPECT_EQ(out.str(), expected);
}