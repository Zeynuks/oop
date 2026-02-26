#pragma once

#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

using Numbers = std::vector<double>;

Numbers ReadNumbers();
void ProcessNumbers(std::vector<double>& numbers);
void PrintSortedNumbers(const std::vector<double>& numbers);