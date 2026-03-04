#include "BaseMoneyStorage.hpp"
#include "errors/WalletError.hpp"

#include <iostream>
#include <limits>

BaseMoneyStorage::BaseMoneyStorage(const Money initialAmount)
	: m_id(m_nextId++)
{
	BaseMoneyStorage::Deposit(initialAmount);
}

StorageId BaseMoneyStorage::GetId() const
{
	return m_id;
}

Money BaseMoneyStorage::GetBalance() const
{
	return m_balance;
}

void BaseMoneyStorage::Deposit(const Money amount)
{
	if (amount < 0)
	{
		throw WalletError("Withdrawal amount must be positive.");
	}

	if (std::numeric_limits<Money>::max() - m_balance < amount)
	{
		throw WalletError("Balance limit exceeded.");
	}

	m_balance += amount;
}

Money BaseMoneyStorage::Withdraw(const Money amount)
{
	if (amount < 0)
	{
		throw WalletError("Withdrawal amount must be positive.");
	}

	if (m_balance < amount)
	{
		throw WalletError("Not enough money.");
	}

	m_balance -= amount;
	return amount;
}