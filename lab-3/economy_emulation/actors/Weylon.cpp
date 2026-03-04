#include "Weylon.hpp"

#include "EconomyContext.hpp"
#include "MoneyTransfer.hpp"
#include "Random.hpp"

#include <iostream>

Weylon::Weylon(const ActorId id, Bank& bank, const ActorId apuId)
	: Person(id, "Weylon", bank)
	, m_apuId(apuId)
{
	Person::OpenBankAccount();
}

void Weylon::Tick(EconomyContext& context)
{
	if (RandomBool() || m_bankAccount)
	{
		CloseBankAccount();
		std::cout << GetName() << ": " << "Close account" << std::endl;

		auto& apu = context.GetActor<IFinancialActor>(m_apuId);
		apu.ReceiveBankTransfer(m_wallet, 75);
		std::cout << GetName() << ": " << "Buy products" << std::endl;
	}
	else if (m_bankAccount)
	{
		IMoneyStorage& account = GetBankAccount();

		auto& apu = context.GetActor<IFinancialActor>(m_apuId);
		apu.ReceiveBankTransfer(account, 75);
		std::cout << GetName() << ": " << "Buy products" << std::endl;
	}
	else
	{
		OpenBankAccount();
		IMoneyStorage& account = GetBankAccount();

		auto& apu = context.GetActor<IFinancialActor>(m_apuId);
		apu.ReceiveBankTransfer(account, 75);
		std::cout << GetName() << ": " << "Buy products" << std::endl;
	}
}