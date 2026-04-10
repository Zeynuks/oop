#pragma once

#include "money_storage/IMoneyStorage.hpp"

class IBankAccountHolder
{
public:
	virtual ~IBankAccountHolder() = default;

	virtual IMoneyStorage& GetBankAccount() const = 0;
	virtual void OpenBankAccount() = 0;
	virtual void CloseBankAccount(IMoneyStorage& to) = 0;
};