#pragma once

#include "IMoneyStorage.hpp"

class IStealable
{
public:
	virtual ~IStealable() = default;
	virtual IMoneyStorage& GetStorageToSteal() = 0;
};