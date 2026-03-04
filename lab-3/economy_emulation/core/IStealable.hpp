#pragma once

#include "money_storage/IMoneyStorage.hpp"

class IStealable
{
public:
	virtual ~IStealable() = default;
	virtual IMoneyStorage& GetStorageToSteal() = 0;
};