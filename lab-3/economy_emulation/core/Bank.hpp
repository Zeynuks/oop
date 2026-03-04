#pragma once

#include "money_storage/BankAccount.hpp"

#include <unordered_map>

using AccountId = unsigned long long;

class Bank
{
public:
	explicit Bank(IMoneyStorage& bankStorage);

	Bank(const Bank&) = delete;
	Bank& operator=(const Bank&) = delete;

	[[nodiscard]] IMoneyStorage& OpenAccount();
	void CloseAccount(const IMoneyStorage& account);

private:
	IMoneyStorage& m_bankStorage;
	std::unordered_map<AccountId, BankAccount> m_accounts;
};