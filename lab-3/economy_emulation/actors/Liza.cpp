#include "Liza.hpp"
#include "EconomyContext.hpp"
#include "MoneyTransfer.hpp"

#include <iostream>

Liza::Liza(const ActorId id, Bank& bank, const ActorId apuId)
	: BaseActor(id, "Liza", bank)
	, m_apuId(apuId)
{
}

void Liza::Tick(EconomyContext& context)
{
	auto& apu = context.GetActor<IFinancialActor>(m_apuId);
	apu.ReceiveCash(m_wallet, 35);
	std::cout << GetName() << ": " << "Buy products" << std::endl;
}