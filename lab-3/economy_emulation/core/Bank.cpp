#include "Bank.hpp"

Bank::Bank(IMoneyStorage& bankStorage)
	: m_bankStorage(bankStorage)
{
}

IMoneyStorage& Bank::OpenAccount()
{
	auto newAccount = BankAccount();
	m_accounts.emplace(newAccount.GetId(), newAccount);

	return m_accounts.at(newAccount.GetId());
}

void Bank::CloseAccount(const IMoneyStorage& account)
{
	m_accounts.erase(account.GetId());
}