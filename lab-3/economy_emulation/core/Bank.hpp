#pragma once

#include "money_storage/BankAccount.hpp"

#include <unordered_map>

using AccountId = unsigned long long;

class Bank
{
public:
	explicit Bank(IMoneyStorage& bankStorage)
		: m_bankStorage(bankStorage)
	{
	}

	Bank(const Bank&) = delete;
	Bank& operator=(const Bank&) = delete;

	IMoneyStorage& OpenAccount()
	{
		auto newAccount = BankAccount();
		m_accounts.emplace(newAccount.GetId(), newAccount);

		return m_accounts.at(newAccount.GetId());
	}

	void CloseAccount(const IMoneyStorage& account)
	{
		m_accounts.erase(account.GetId());
	}

private:
	IMoneyStorage& m_bankStorage;
	std::unordered_map<AccountId, BankAccount> m_accounts;
};