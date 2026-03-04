#include "Liza.hpp"
#include "EconomyContext.hpp"

#include <iostream>

Liza::Liza(const ActorId id, Bank& bank, const ActorId apuId)
	: Person(id, "Liza", bank)
	, m_apuId(apuId)
{
}

void Liza::Tick(EconomyContext& context)
{
	auto& apu = context.GetActor<IMoneyReceiver>(m_apuId);
	apu.ReceiveCash(GetWallet(), 35);
	std::cout << GetName() << ": " << "Buy products" << std::endl;
}