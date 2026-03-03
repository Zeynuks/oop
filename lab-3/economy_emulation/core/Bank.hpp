#pragma once

#include "BankAccount.hpp"

#include <memory>
#include <vector>

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
	std::vector<std::unique_ptr<BankAccount>> m_accounts;
};