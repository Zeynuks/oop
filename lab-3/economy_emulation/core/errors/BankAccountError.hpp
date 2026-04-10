#pragma once

#include <stdexcept>

class BankAccountError : public std::runtime_error
{
public:
	using runtime_error::runtime_error;
};