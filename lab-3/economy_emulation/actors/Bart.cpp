#include "Bart.hpp"

#include "EconomyContext.hpp"
#include "Random.hpp"

#include <iostream>

Bart::Bart(const ActorId id, Bank& bank, const ActorId apuId)
	: Person(id, "Bart", bank)
	, m_apuId(apuId)
{
}

void Bart::Tick(EconomyContext& context)
{
	auto& apu = context.GetActor<IMoneyReceiver>(m_apuId);
	apu.ReceiveCash(m_wallet, 40);
	std::cout << GetName() << ": " << "Buy products" << std::endl;
}

IMoneyStorage& Bart::GetStorageToSteal()
{
	return m_wallet;
}