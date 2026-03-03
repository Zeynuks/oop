#include "Nelson.hpp"

#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"
#include "Random.hpp"

#include <iostream>

Nelson::Nelson(const ActorId id, Bank& bank, const ActorId bartId, const ActorId apuId)
	: BaseActor(id, "Nelson", bank)
	, m_bartId(bartId)
	, m_apuId(apuId)
{
}

void Nelson::Tick(EconomyContext& context)
{
	if (RandomBool())
	{
		IActor& bart = context.GetActor(m_bartId);
		IMoneyStorage& bartWallet = bart.StealMoney();

		ReceiveCash(bartWallet, RandomMoney(20, 50));
		std::cout << GetName() << ": " << "Steal money from Bart" << std::endl;

		IActor& apu = context.GetActor(m_apuId);

		apu.ReceiveCash(m_wallet, 30);
		std::cout << GetName() << ": " << "Buy cigarettes" << std::endl;
	}
}