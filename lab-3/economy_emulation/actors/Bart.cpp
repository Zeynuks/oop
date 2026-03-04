#include "Bart.hpp"

#include "EconomyContext.hpp"
#include "MoneyTransfer.hpp"
#include "Random.hpp"

#include <iostream>

Bart::Bart(const ActorId id, Bank& bank, const ActorId apuId)
	: BaseActor(id, "Bart", bank)
	, m_apuId(apuId)
{
}

void Bart::Tick(EconomyContext& context)
{
	auto& apu = context.GetActor<IFinancialActor>(m_apuId);
	apu.ReceiveCash(m_wallet, 40);
	std::cout << GetName() << ": " << "Buy products" << std::endl;
}

void Bart::StealMoney(IFinancialActor& stealer, const Money amount)
{
	stealer.ReceiveCash(m_wallet, amount);
}