#pragma once

#include "../errors/MoneyStorageError.hpp"
#include "IMoneyStorage.hpp"

#include <limits>

class BaseMoneyStorage : public IMoneyStorage
{
public:
	explicit BaseMoneyStorage(const Money initialAmount = 0)
		: m_id(m_nextId++)
	{
		BaseMoneyStorage::Deposit(initialAmount);
	}

	StorageId GetId() const override
	{
		return m_id;
	}

	Money GetBalance() const override
	{
		return m_balance;
	}

private:
	static inline StorageId m_nextId = 0;
	StorageId m_id;
	Money m_balance = 0;

	void Deposit(const Money amount) override
	{
		if (amount < 0)
		{
			throw MoneyStorageError("Withdrawal amount must be positive.");
		}

		if (std::numeric_limits<Money>::max() - m_balance < amount)
		{
			throw MoneyStorageError("Balance limit exceeded.");
		}

		m_balance += amount;
	}

	Money Withdraw(const Money amount) override
	{
		if (amount < 0)
		{
			throw MoneyStorageError("Withdrawal amount must be positive.");
		}

		if (m_balance < amount)
		{
			throw MoneyStorageError("Not enough money.");
		}

		m_balance -= amount;
		return amount;
	}
};