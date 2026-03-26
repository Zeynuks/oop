#include <gtest/gtest.h>
#include <set>
#include <vector>

#include "../prime_generator.h"

TEST(GeneratePrimeNumbersSetTest, UpperBoundLessThanTwo_ReturnsEmptySet)
{
    EXPECT_TRUE(GeneratePrimeNumbersSet(-10).empty());
    EXPECT_TRUE(GeneratePrimeNumbersSet(-1).empty());
    EXPECT_TRUE(GeneratePrimeNumbersSet(0).empty());
    EXPECT_TRUE(GeneratePrimeNumbersSet(1).empty());
}

TEST(GeneratePrimeNumbersSetTest, UpperBoundEqualsTwo)
{
	const std::set expected = {2};
    EXPECT_EQ(GeneratePrimeNumbersSet(2), expected);
}

TEST(GeneratePrimeNumbersSetTest, UpperBoundEqualsThree)
{
	const std::set expected = {2, 3};
    EXPECT_EQ(GeneratePrimeNumbersSet(3), expected);
}

TEST(GeneratePrimeNumbersSetTest, SmallRangeUpToTen)
{
	const std::set expected = {2, 3, 5, 7};
    EXPECT_EQ(GeneratePrimeNumbersSet(10), expected);
}

TEST(GeneratePrimeNumbersSetTest, UpperBoundIsPrime_IncludedInResult)
{
	const auto result = GeneratePrimeNumbersSet(13);
    EXPECT_TRUE(result.contains(13));
}

TEST(GeneratePrimeNumbersSetTest, UpperBoundIsComposite_NotIncluded)
{
	const auto result = GeneratePrimeNumbersSet(12);
    EXPECT_FALSE(result.contains(12));
}

TEST(GeneratePrimeNumbersSetTest, NoCompositeNumbersPresent)
{
	const auto result = GeneratePrimeNumbersSet(30);

	const std::vector composites = {4, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20, 21, 22, 24, 25, 26, 27, 28};

    for (int c : composites)
    {
        EXPECT_FALSE(result.contains(c));
    }
}

TEST(GeneratePrimeNumbersSetTest, MediumRangeUpToHundred)
{
	const auto result = GeneratePrimeNumbersSet(100);

	const std::vector expectedPrimes = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97
    };

    EXPECT_EQ(result.size(), expectedPrimes.size());

    for (int prime : expectedPrimes)
    {
        EXPECT_TRUE(result.contains(prime));
    }
}

TEST(GeneratePrimeNumbersSetTest, DeterministicOutput)
{
	const auto first = GeneratePrimeNumbersSet(200);
	const auto second = GeneratePrimeNumbersSet(200);

    EXPECT_EQ(first, second);
}

TEST(GeneratePrimeNumbersSetTest, LargeUpperBound_SanityCheck)
{
	const auto result = GeneratePrimeNumbersSet(1000);

    EXPECT_TRUE(result.contains(997));
    EXPECT_FALSE(result.contains(1000));
    EXPECT_GT(result.size(), 0);
}
