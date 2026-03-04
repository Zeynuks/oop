#include "BaseActor.hpp"
#include "IMoneyStorage.hpp"
#include "MoneyTransfer.hpp"
#include "errors/ActorError.hpp"

BaseActor::BaseActor(const ActorId id, const std::string& name, Bank& bank, const Money initialWalletCash)
	: m_id(id)
	, m_name(name)
	, m_bank(bank)
	, m_wallet(initialWalletCash)
{
}

ActorId BaseActor::GetId() const
{
	return m_id;
}

std::string BaseActor::GetName() const
{
	return m_name;
}

IMoneyStorage& BaseActor::GetBankAccount()
{
	if (!m_bankAccount)
	{
		throw ActorError("No bank account");
	}

	return m_bankAccount->get();
}

void BaseActor::OpenBankAccount(const Money initialAmount)
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

void BaseActor::CloseBankAccount()
{
	IMoneyStorage& account = m_bank.OpenAccount();
	const Money amount = m_bank.CloseAccount(account);
	ReceiveCash(m_wallet, amount);
	m_bankAccount = std::nullopt;
}

void BaseActor::ReceiveBankTransfer(IMoneyStorage& from, const Money amount)
{
	MoneyTransfer transfer(from, amount);

	IMoneyStorage& account = GetBankAccount();
	transfer.To(account);
}
void BaseActor::ReceiveCash(IMoneyStorage& from, const Money amount)
{
	MoneyTransfer(from, amount).To(m_wallet);
}