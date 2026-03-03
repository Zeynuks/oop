#include "Bart.hpp"

#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"
#include "Random.hpp"

Bart::Bart(const ActorId id, Bank& bank, const ActorId apuId)
	: BaseActor(id, "Berns", bank)
	, m_apuId(apuId)
{
}

void Bart::Tick(EconomyContext& context)
{
	IActor& apu = context.GetActor(m_apuId);
	apu.ReceiveCash(m_wallet, 40);
}

IMoneyStorage& Bart::StealMoney()
{
	return m_wallet;
}