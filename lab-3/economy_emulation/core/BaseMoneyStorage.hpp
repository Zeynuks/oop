#pragma once

#include "money_storage/IMoneyStorage.hpp"

class BaseMoneyStorage : public IMoneyStorage
{
public:
	explicit BaseMoneyStorage(Money initialAmount = 0);
	StorageId GetId() const override;
	Money GetBalance() const override;

private:
	static inline StorageId m_nextId = 0;
	StorageId m_id;
	Money m_balance;

	void Deposit(Money amount) override;
	Money Withdraw(Money amount) override;
};