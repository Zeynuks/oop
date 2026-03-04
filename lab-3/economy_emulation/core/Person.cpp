#include "Person.hpp"
#include "money_storage/IMoneyStorage.hpp"
#include "money_storage/MoneyTransfer.hpp"

Person::Person(const ActorId id, const std::string& name, Bank& bank)
	: BaseActor(id, name)
	, BaseBankAccountHolder(bank)
	, BaseWalletHolder(0)
{
}

void Person::ReceiveBankTransfer(IMoneyStorage& from, const Money amount)
{
	IMoneyStorage& account = GetBankAccount();
	MoneyTransfer(from, amount).To(account);
}

void Person::ReceiveCash(IMoneyStorage& from, const Money amount)
{
	MoneyTransfer(from, amount).To(GetWallet());
}

IMoneyStorage& Person::GetBankAccount() const
{
	return BaseBankAccountHolder::GetBankAccount();
}

IMoneyStorage& Person::GetWallet()
{
	return BaseWalletHolder::GetWallet();
}