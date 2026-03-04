#include "BaseBankAccountHolder.hpp"
#include "errors/BankAccountError.hpp"
#include "money_storage/MoneyTransfer.hpp"

BaseBankAccountHolder::BaseBankAccountHolder(Bank& m_bank)
	: m_bank(m_bank)
{
}

IMoneyStorage& BaseBankAccountHolder::GetBankAccount() const
{
	if (!m_bankAccount)
	{
		throw BankAccountError("No bank account");
	}

	return m_bankAccount->get();
}

void BaseBankAccountHolder::OpenBankAccount()
{
	if (!m_bankAccount.has_value())
	{
		IMoneyStorage& account = m_bank.OpenAccount();
		m_bankAccount = std::ref(account);
	}
}

void BaseBankAccountHolder::CloseBankAccount(IMoneyStorage& to)
{
	IMoneyStorage& account = GetBankAccount();
	MoneyTransfer(account, account.GetBalance()).To(to);
	m_bank.CloseAccount(account);

	m_bankAccount = std::nullopt;
}