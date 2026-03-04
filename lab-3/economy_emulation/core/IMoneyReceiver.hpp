#pragma once

#include "money_storage/IMoneyStorage.hpp"

class IMoneyReceiver
{
public:
	virtual ~IMoneyReceiver() = default;

	virtual void ReceiveBankTransfer(IMoneyStorage& from, Money amount) = 0;
	virtual void ReceiveCash(IMoneyStorage& from, Money amount) = 0;
};