#pragma once

#include "../core/EconomyContext.hpp"
#include "../core/Person.hpp"

class Weylon : public Person
{
public:
	Weylon(const ActorId id, Bank& bank, const ActorId apuId)
		: Person(id, "Weylon", bank)
		, m_apuId(apuId)
	{
		Person::OpenBankAccount();
	}

	void Tick(EconomyContext& context) override
	{
		if (RandomBool() || GetBankAccount().GetId())
		{
			CloseBankAccount(GetWallet());
			std::cout << GetName() << ": " << "Close account" << std::endl;

			auto& apu = context.GetActor<IMoneyReceiver>(m_apuId);
			apu.ReceiveBankTransfer(GetWallet(), 75);
			std::cout << GetName() << ": " << "Buy products" << std::endl;
		}
		else if (GetBankAccount().GetId())
		{
			IMoneyStorage& account = GetBankAccount();

			auto& apu = context.GetActor<IMoneyReceiver>(m_apuId);
			apu.ReceiveBankTransfer(account, 75);
			std::cout << GetName() << ": " << "Buy products" << std::endl;
		}
		else
		{
			OpenBankAccount();
			IMoneyStorage& account = GetBankAccount();

			auto& apu = context.GetActor<IMoneyReceiver>(m_apuId);
			apu.ReceiveBankTransfer(account, 75);
			std::cout << GetName() << ": " << "Buy products" << std::endl;
		}
	}

private:
	ActorId m_apuId;
};