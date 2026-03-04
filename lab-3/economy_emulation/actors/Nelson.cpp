#include "Nelson.hpp"

#include "EconomyContext.hpp"
#include "IStealable.hpp"
#include "MoneyTransfer.hpp"
#include "Random.hpp"

#include <iostream>

Nelson::Nelson(const ActorId id, Bank& bank, const ActorId bartId, const ActorId apuId)
	: Person(id, "Nelson", bank)
	, m_bartId(bartId)
	, m_apuId(apuId)
{
}

void Nelson::Tick(EconomyContext& context)
{
	if (RandomBool())
	{
		auto& bart = context.GetActor<IStealable>(m_bartId);
		IMoneyStorage& bartWallet = bart.GetStorageToSteal();

		ReceiveCash(bartWallet, RandomMoney(20, 50));
		std::cout << GetName() << ": " << "Steal money from Bart" << std::endl;

		auto& apu = context.GetActor<IFinancialActor>(m_apuId);
		apu.ReceiveCash(m_wallet, 30);
		std::cout << GetName() << ": " << "Buy cigarettes" << std::endl;
	}
}