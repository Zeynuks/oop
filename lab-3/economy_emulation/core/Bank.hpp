#pragma once

#include "BankAccount.hpp"

#include <list>
#include <memory>

class Bank
{
public:
	explicit Bank();

	Bank(const Bank&) = delete;
	Bank& operator=(const Bank&) = delete;

	[[nodiscard]] IMoneyStorage& OpenAccount();
	[[nodiscard]] IMoneyStorage& OpenAccount(Money amount);
	[[nodiscard]] Money CloseAccount(IMoneyStorage& account);

private:
	std::list<std::unique_ptr<BankAccount>> m_accounts;
};