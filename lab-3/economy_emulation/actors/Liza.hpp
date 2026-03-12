#pragma once

#include "../core/EconomyContext.hpp"
#include "../core/Person.hpp"

class Liza : public Person
{
public:
	Liza(const ActorId id, Bank& bank, const ActorId apuId)
		: Person(id, "Liza", bank)
		, m_apuId(apuId)
	{
	}

	void Tick(EconomyContext& context) override
	{
		auto& apu = context.GetActor<IMoneyReceiver>(m_apuId);
		apu.ReceiveCash(GetWallet(), 35);
		std::cout << GetName() << ": " << "Buy products" << std::endl;
	}

private:
	ActorId m_apuId;
};