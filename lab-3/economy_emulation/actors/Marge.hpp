#pragma once

#include "../core/EconomyContext.hpp"
#include "../core/Person.hpp"

class Marge : public Person
{
public:
	Marge(const ActorId id, Bank& bank, const ActorId apuId)
		: Person(id, "Marge", bank)
		, m_apuId(apuId)
	{
		Person::OpenBankAccount();
	}

	void Tick(EconomyContext& context) override
	{
		IMoneyStorage& account = GetBankAccount();

		auto& apu = context.GetActor<IMoneyReceiver>(m_apuId);
		apu.ReceiveBankTransfer(account, 75);
		std::cout << GetName() << ": " << "Buy products" << std::endl;
	}

private:
	ActorId m_apuId;
};