#include "numbers.h"

Numbers ReadNumbers()
{
	Numbers numbers;

	double number;
	while (std::cin >> number)
	{
		numbers.push_back(number);
	}
	if (!std::cin.eof())
	{
		throw std::invalid_argument("Invalid");
	}

	return numbers;
}

void ProcessNumbers(std::vector<double>& numbers)
{
	auto [sum, count] = std::accumulate(
		numbers.begin(),
		numbers.end(),
		std::pair<double,int>{0,0},
		[](auto accumulate, double x) {
			if (x > 0) {
				accumulate.first += x;
				accumulate.second += 1;
			}
			return accumulate;
		}
	);

	const double mean = count > 0 ? sum / count : 0;

	for (double& x : numbers)
	{
		x += mean;
	}
}

void PrintSortedNumbers(const std::vector<double>& numbers)
{
	std::vector<double> sorted = numbers;
	std::ranges::sort(sorted);

	std::cout << std::fixed << std::setprecision(3);
	std::cout << std::endl;

	for (size_t i = 0; i < sorted.size(); ++i)
	{
		if (i > 0)
		{
			std::cout << " ";
		}
		std::cout << sorted[i];
	}
	std::cout << std::endl;
}
