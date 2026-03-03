#pragma once

#include <stdexcept>

class WalletError : public std::runtime_error
{
public:
	using runtime_error::runtime_error;
};