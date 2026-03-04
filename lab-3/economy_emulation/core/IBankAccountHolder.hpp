#pragma once

#include "money_storage/IMoneyStorage.hpp"

class IBankAccountHolder
{
public:
	virtual ~IBankAccountHolder() = default;

	virtual IMoneyStorage& GetBankAccount() const = 0;
	virtual void OpenBankAccount(Money initialAmount = 0) = 0;
	virtual void CloseBankAccount() = 0;
};