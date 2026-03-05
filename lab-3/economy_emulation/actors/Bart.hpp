#pragma once

#include "../core/EconomyContext.hpp"
#include "../core/IStealable.hpp"
#include "../core/Person.hpp"

#include <iostream>

class Bart : public Person
	, public IStealable
{
public:
	Bart(const ActorId id, Bank& bank, const ActorId apuId)
		: Person(id, "Bart", bank)
		, m_apuId(apuId)
	{
	}

	void Tick(EconomyContext& context) override
	{
		auto& apu = context.GetActor<IMoneyReceiver>(m_apuId);
		apu.ReceiveCash(GetWallet(), 40);
		std::cout << GetName() << ": " << "Buy products" << std::endl;
	}

	IMoneyStorage& GetStorageToSteal() override
	{
		return GetWallet();
	}

private:
	ActorId m_apuId;
};