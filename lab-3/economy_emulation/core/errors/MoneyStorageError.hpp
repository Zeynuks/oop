#pragma once

#include <stdexcept>

class MoneyStorageError : public std::runtime_error
{
public:
	using runtime_error::runtime_error;
};