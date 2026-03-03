#include "Homer.hpp"

#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"
#include "Random.hpp"

#include <iostream>

Homer::Homer(const ActorId id, Bank& bank, const ActorId margeId, const ActorId bernsId, const ActorId bartId, const ActorId lisaId, const ActorId chesterId)
	: BaseActor(id, "Homer", bank)
	, m_margeId(margeId)
	, m_bernsId(bernsId)
	, m_bartId(bartId)
	, m_lizaId(lisaId)
	, m_chesterId(chesterId)
{
	IMoneyStorage& account = m_bank.OpenAccount();
	m_bankAccount = std::ref(account);
}

void Homer::Tick(EconomyContext& context)
{
	IMoneyStorage& account = GetBankAccount();

	IActor& marge = context.GetActor(m_margeId);
	IActor& berns = context.GetActor(m_bernsId);
	IActor& liza = context.GetActor(m_lizaId);
	IActor& bart = context.GetActor(m_bartId);

	marge.ReceiveBankTransfer(account, RandomMoney(50, 100));
	std::cout << GetName() << ": " << "Send money to Marge" << std::endl;
	berns.ReceiveBankTransfer(account, RandomMoney(20, 75));
	std::cout << GetName() << ": " << "Paid for eclecticism" << std::endl;
	liza.ReceiveCash(account, RandomMoney(0, 50));
	std::cout << GetName() << ": " << "Get money to Liza" << std::endl;
	bart.ReceiveCash(account, RandomMoney(0, 50));
	std::cout << GetName() << ": " << "Get money to Bart" << std::endl;
}

IMoneyStorage& Homer::StealMoney()
{
	return GetBankAccount();
}