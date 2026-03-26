#include "prime_generator.h"

#include <cmath>
#include <set>
#include <vector>

std::set<int> GeneratePrimeNumbers_Base(const int upperBound)
{
	if (upperBound < 2)
	{
		return {};
	}

	std::vector isPrime(upperBound + 1, true);
	isPrime[0] = isPrime[1] = false;

	const int limit = static_cast<int>(std::sqrt(upperBound));

	for (int p = 2; p <= limit; ++p)
	{
		if (isPrime[p])
		{
			for (int multiple = p * p; multiple <= upperBound; multiple += p)
			{
				isPrime[multiple] = false;
			}
		}
	}

	std::set<int> primes;
	for (int i = 2; i <= upperBound; ++i)
	{
		if (isPrime[i])
		{
			primes.insert(i);
		}
	}

	return primes;
}

std::set<int> GeneratePrimeNumbers_Odd(const int upperBound)
{
	if (upperBound < 2)
	{
		return {};
	}

	const int size = (upperBound - 1) / 2;
	std::vector isPrime(size, true);

	const int limit = static_cast<int>(std::sqrt(upperBound));

	for (int i = 0; 2 * i + 3 <= limit; ++i)
	{
		if (isPrime[i])
		{
			const int p = 2 * i + 3;
			const int start = (p * p - 3) / 2;

			for (int j = start; j < size; j += p)
			{
				isPrime[j] = false;
			}
		}
	}

	std::set<int> primes;
	primes.insert(2);

	for (int i = 0; i < size; ++i)
	{
		if (isPrime[i])
		{
			primes.insert(2 * i + 3);
		}
	}

	return primes;
}

std::vector<int> GeneratePrimeNumbers_NoSet(const int upperBound)
{
	if (upperBound < 2)
	{
		return {};
	}

	std::vector isPrime(upperBound + 1, true);
	isPrime[0] = isPrime[1] = false;

	const int limit = static_cast<int>(std::sqrt(upperBound));

	for (int p = 2; p <= limit; ++p)
	{
		if (isPrime[p])
		{
			for (int multiple = p * p; multiple <= upperBound; multiple += p)
			{
				isPrime[multiple] = false;
			}
		}
	}

	std::vector<int> primes;
	for (int i = 2; i <= upperBound; ++i)
	{
		if (isPrime[i])
		{
			primes.push_back(i);
		}
	}

	return primes;
}

std::vector<int> GeneratePrimeNumbers_Optimized(const int upperBound)
{
	if (upperBound < 2)
	{
		return {};
	}

	const int size = (upperBound - 1) / 2;
	std::vector<char> isPrime(size, 1);

	const int limit = static_cast<int>(std::sqrt(upperBound));

	for (int i = 0; 2 * i + 3 <= limit; ++i)
	{
		if (isPrime[i])
		{
			const int p = 2 * i + 3;
			const int start = (p * p - 3) / 2;

			for (int j = start; j < size; j += p)
			{
				isPrime[j] = 0;
			}
		}
	}

	std::vector<int> primes;
	primes.reserve(upperBound / std::log(upperBound));

	primes.push_back(2);

	for (int i = 0; i < size; ++i)
	{
		if (isPrime[i])
		{
			primes.push_back(2 * i + 3);
		}
	}

	return primes;
}
