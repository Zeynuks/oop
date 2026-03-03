#include "Liza.hpp"
#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"

Liza::Liza(const ActorId id, Bank& bank, const ActorId apuId)
	: BaseActor(id, "Liza", bank)
	, m_apuId(apuId)
{
}

void Liza::Tick(EconomyContext& context)
{
	IActor& apu = context.GetActor(m_apuId);
	apu.ReceiveCash(m_wallet, 35);
}