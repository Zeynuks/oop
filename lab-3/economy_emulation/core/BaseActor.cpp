#include "BaseActor.hpp"
#include "IMoneyStorage.hpp"
#include "MoneyTransfer.hpp"
#include "errors/ActorError.hpp"

BaseActor::BaseActor(const ActorId id, const std::string& name, Bank& bank)
	: m_id(id)
	, m_name(name)
	, m_bank(bank)
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

IMoneyStorage& BaseActor::StealMoney()
{
	throw ActorError("You cannot steal from that Actor!");
}

void BaseActor::WithdrawMoney(const Money amount)
{
	IMoneyStorage& account = GetBankAccount();

	MoneyTransfer transfer(account, amount);
	transfer.To(m_wallet);
}

void BaseActor::DepositMoney(const Money amount)
{
	IMoneyStorage& account = GetBankAccount();

	MoneyTransfer transfer(m_wallet, amount);
	transfer.To(account);
}