#include "Chester.hpp"

#include "EconomyContext.hpp"
#include "IStealable.hpp"
#include "Liza.hpp"
#include "Random.hpp"

#include <iostream>

Chester::Chester(const ActorId id, Bank& bank, const ActorId homerId, const ActorId apuId)
	: Person(id, "Chester", bank)
	, m_homerId(homerId)
	, m_apuId(apuId)
{
	Person::OpenBankAccount();
}

void Chester::Tick(EconomyContext& context)
{
	if (RandomBool())
	{
		IMoneyStorage& account = GetBankAccount();

		auto& homer = context.GetActor<IStealable>(m_homerId);
		auto& homerBankAccount = homer.GetStorageToSteal();
		ReceiveBankTransfer(homerBankAccount, RandomMoney(100, 150));
		std::cout << GetName() << ": " << "Steal money from Homer" << std::endl;

		auto& apu = context.GetActor<IFinancialActor>(m_apuId);
		apu.ReceiveCash(account, 100);
		std::cout << GetName() << ": " << "Buy products" << std::endl;
	}
}