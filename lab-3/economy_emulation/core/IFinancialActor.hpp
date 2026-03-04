#pragma once

#include "IMoneyStorage.hpp"

class IFinancialActor
{
public:
	virtual ~IFinancialActor() = default;

	virtual void ReceiveBankTransfer(IMoneyStorage& from, Money amount) = 0;
	virtual void ReceiveCash(IMoneyStorage& from, Money amount) = 0;

	virtual bool HasBankAccount() = 0;

	virtual void OpenBankAccount(Money initialAmount = 0) = 0;
	virtual void CloseBankAccount() = 0;
};