#include "prime_generator.h"

#include <set>
#include <vector>
#include <cmath>

std::set<int> GeneratePrimeNumbersSet(const int upperBound)
{
	if (upperBound < 2)
	{
		return {};
	}

	const int size = (upperBound - 1) / 2;
	std::vector isPrime(size, true);

	const int limit = static_cast<int>(std::sqrt(upperBound));

	for (int i = 0; (2 * i + 3) <= limit; ++i)
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