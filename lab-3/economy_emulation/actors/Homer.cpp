#include "Homer.hpp"

#include "EconomyContext.hpp"
#include "MoneyTransfer.hpp"
#include "Random.hpp"

#include <iostream>

Homer::Homer(const ActorId id, Bank& bank, const ActorId margeId, const ActorId bernsId, const ActorId bartId, const ActorId lisaId, const ActorId chesterId)
	: Person(id, "Homer", bank)
	, m_margeId(margeId)
	, m_bernsId(bernsId)
	, m_bartId(bartId)
	, m_lizaId(lisaId)
	, m_chesterId(chesterId)
{
	Person::OpenBankAccount();
}

void Homer::Tick(EconomyContext& context)
{
	IMoneyStorage& account = GetBankAccount();

	auto& marge = context.GetActor<IFinancialActor>(m_margeId);
	marge.ReceiveBankTransfer(account, RandomMoney(50, 100));
	std::cout << GetName() << ": " << "Send money to Marge" << std::endl;

	auto& berns = context.GetActor<IFinancialActor>(m_bernsId);
	berns.ReceiveBankTransfer(account, RandomMoney(20, 75));
	std::cout << GetName() << ": " << "Paid for eclecticism" << std::endl;

	auto& liza = context.GetActor<IFinancialActor>(m_lizaId);
	liza.ReceiveCash(account, RandomMoney(0, 50));
	std::cout << GetName() << ": " << "Get money to Liza" << std::endl;

	auto& bart = context.GetActor<IFinancialActor>(m_bartId);
	bart.ReceiveCash(account, RandomMoney(0, 50));
	std::cout << GetName() << ": " << "Get money to Bart" << std::endl;
}

IMoneyStorage& Homer::GetStorageToSteal()
{
	return GetBankAccount();
}