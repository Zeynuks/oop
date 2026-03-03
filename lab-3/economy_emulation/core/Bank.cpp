#include "Bank.hpp"

Bank::Bank()
{
}

IMoneyStorage& Bank::OpenAccount()
{
	m_accounts.push_back(std::make_unique<BankAccount>());
	return *m_accounts.back();
}

IMoneyStorage& Bank::OpenAccount(Money amount)
{
	m_accounts.push_back(std::make_unique<BankAccount>(amount));
	return *m_accounts.back();
}


Money Bank::CloseAccount(IMoneyStorage& account)
{
	const auto it = std::ranges::find_if(m_accounts,
		[&account](const std::unique_ptr<BankAccount>& ptr) {
			return ptr.get() == &account;
		});

	if (it == m_accounts.end()) {
		return 0;
	}

	const Money balance = it->get()->GetBalance();
	m_accounts.erase(it);

	return balance;
}