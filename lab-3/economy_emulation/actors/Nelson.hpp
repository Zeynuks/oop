#pragma once

#include "../core/EconomyContext.hpp"
#include "../core/Person.hpp"

class Nelson : public Person
{
public:
	Nelson(const ActorId id, Bank& bank, const ActorId bartId, const ActorId apuId)
		: Person(id, "Nelson", bank)
		, m_bartId(bartId)
		, m_apuId(apuId)
	{
	}

	void Tick(EconomyContext& context) override
	{
		if (RandomBool())
		{
			auto& bart = context.GetActor<IStealable>(m_bartId);
			IMoneyStorage& bartWallet = bart.GetStorageToSteal();

			ReceiveCash(bartWallet, RandomMoney(20, 50));
			std::cout << GetName() << ": " << "Steal money from Bart" << std::endl;

			auto& apu = context.GetActor<IMoneyReceiver>(m_apuId);
			apu.ReceiveCash(GetWallet(), 30);
			std::cout << GetName() << ": " << "Buy cigarettes" << std::endl;
		}
	}

private:
	ActorId m_bartId;
	ActorId m_apuId;
};