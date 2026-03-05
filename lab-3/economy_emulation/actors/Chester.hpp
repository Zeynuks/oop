#pragma once

#include "../core/EconomyContext.hpp"
#include "../core/IStealable.hpp"
#include "../core/Person.hpp"
#include "Random.hpp"

class IStealable;
class Chester : public Person
{
public:
	Chester(const ActorId id, Bank& bank, const ActorId homerId, const ActorId apuId)
		: Person(id, "Chester", bank)
		, m_homerId(homerId)
		, m_apuId(apuId)
	{
		Person::OpenBankAccount();
	}

	void Tick(EconomyContext& context) override
	{
		if (RandomBool())
		{
			IMoneyStorage& account = GetBankAccount();

			auto& homer = context.GetActor<IStealable>(m_homerId);
			auto& homerBankAccount = homer.GetStorageToSteal();
			ReceiveBankTransfer(homerBankAccount, RandomMoney(100, 150));
			std::cout << GetName() << ": " << "Steal money from Homer" << std::endl;

			auto& apu = context.GetActor<IMoneyReceiver>(m_apuId);
			apu.ReceiveCash(account, 100);
			std::cout << GetName() << ": " << "Buy products" << std::endl;
		}
	}

private:
	ActorId m_homerId;
	ActorId m_apuId;
};