#include "IMoneyStorage.hpp"
#include "MoneyTransfer.hpp"
#include "Person.hpp"
#include "errors/ActorError.hpp"

Person::Person(const ActorId id, const std::string& name, Bank& bank, const Money initialWalletCash)
	: m_id(id)
	, m_name(name)
	, m_bank(bank)
	, m_wallet(initialWalletCash)
{
}

ActorId Person::GetId() const
{
	return m_id;
}

std::string Person::GetName() const
{
	return m_name;
}

IMoneyStorage& Person::GetBankAccount() const
{
	if (!m_bankAccount)
	{
		throw ActorError("No bank account");
	}

	return m_bankAccount->get();
}

void Person::OpenBankAccount(const Money initialAmount)
{
	if (!m_bankAccount)
	{
		IMoneyStorage& account = m_bank.OpenAccount(initialAmount);
		m_bankAccount = std::ref(account);

		if (initialAmount > 0)
		{
			MoneyTransfer(m_wallet, initialAmount).To(account);
		}
	}
}

void Person::CloseBankAccount()
{
	IMoneyStorage& account = m_bank.OpenAccount();
	const Money amount = m_bank.CloseAccount(account);
	ReceiveCash(m_wallet, amount);
	m_bankAccount = std::nullopt;
}

bool Person::HasBankAccount()
{
	return m_bankAccount.has_value();
}
void Person::ReceiveBankTransfer(IMoneyStorage& from, const Money amount)
{
	MoneyTransfer transfer(from, amount);

	IMoneyStorage& account = GetBankAccount();
	transfer.To(account);
}
void Person::ReceiveCash(IMoneyStorage& from, const Money amount)
{
	MoneyTransfer(from, amount).To(m_wallet);
}