#include "EconomyContext.hpp"
#include "Random.hpp"
#include "Weylon.hpp"

#include <iostream>

Weylon::Weylon(const ActorId id, Bank& bank, const ActorId apuId)
	: Person(id, "Weylon", bank)
	, m_apuId(apuId)
{
	Person::OpenBankAccount();
}

void Weylon::Tick(EconomyContext& context)
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