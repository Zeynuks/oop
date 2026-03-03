#pragma once

#include "IMoneyStorage.hpp"

class BaseMoneyStorage : public IMoneyStorage
{
public:
	explicit BaseMoneyStorage();
	explicit BaseMoneyStorage(Money amount);
	Money GetBalance() const override;

private:
	void Deposit(Money amount) override;
	Money Withdraw(Money amount) override;

protected:
	Money m_balance;
};