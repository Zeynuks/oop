#include "BaseMoneyStorage.hpp"

#include "Wallet.hpp"
#include "errors/WalletError.hpp"

#include <limits>

BaseMoneyStorage::BaseMoneyStorage(const Money initialAmount)
	: m_balance(initialAmount)
{
}

Money BaseMoneyStorage::GetBalance() const
{
	return m_balance;
}

void BaseMoneyStorage::Deposit(const Money amount)
{
	if (amount <= 0)
	{
		throw WalletError("Withdrawal amount must be positive.");
	}

	if (std::numeric_limits<Money>::max() - m_balance < amount)
	{
		throw WalletError("Overflow error: balance limit exceeded.");
	}

	m_balance += amount;
}

Money BaseMoneyStorage::Withdraw(const Money amount)
{
	if (amount <= 0)
	{
		throw WalletError("Withdrawal amount must be positive.");
	}

	if (m_balance < amount)
	{
		throw WalletError("Insufficient funds: balance cannot be negative.");
	}

	m_balance -= amount;
	return amount;
}