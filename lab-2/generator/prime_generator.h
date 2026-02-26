#pragma once
#include <set>
#include <vector>

std::set<int> GeneratePrimeNumbers_Base(int upperBound);
std::set<int> GeneratePrimeNumbers_Odd(int upperBound);
std::vector<int> GeneratePrimeNumbers_NoSet(int upperBound);
std::vector<int> GeneratePrimeNumbers_Optimized(int upperBound);