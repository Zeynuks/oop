#pragma once

#include "Bank.hpp"

#include <random>

inline Money RandomMoney(const Money min, const Money max)
{
	static std::random_device rd;
	static std::mt19937_64 gen(rd());
	std::uniform_int_distribution dist(min, max);

	return dist(gen);
}

inline bool RandomBool()
{
	static std::random_device rd;
	static std::mt19937_64 gen(rd());
	std::uniform_int_distribution dist(0, 1);

	return dist(gen) == 1;
}