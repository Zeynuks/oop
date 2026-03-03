#include "Weylon.hpp"

#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"
#include "Random.hpp"

#include <iostream>

Weylon::Weylon(const ActorId id, Bank& bank, const ActorId apuId)
	: BaseActor(id, "Waylon", bank)
	, m_apuId(apuId)
{
	BaseActor::OpenBankAccount();
}

void Weylon::Tick(EconomyContext& context)
{
	if (RandomBool() || m_bankAccount)
	{
		CloseBankAccount();
		std::cout << GetName() << ": " << "Close account" << std::endl;

		IActor& apu = context.GetActor(m_apuId);

		apu.ReceiveBankTransfer(m_wallet, 75);
		std::cout << GetName() << ": " << "Buy products" << std::endl;
	} else if (m_bankAccount)
	{
		IMoneyStorage& account = GetBankAccount();

		IActor& apu = context.GetActor(m_apuId);
		apu.ReceiveBankTransfer(account, 75);
		std::cout << GetName() << ": " << "Buy products" << std::endl;
	} else
	{
		OpenBankAccount();
		IMoneyStorage& account = GetBankAccount();

		IActor& apu = context.GetActor(m_apuId);
		apu.ReceiveBankTransfer(account, 75);
		std::cout << GetName() << ": " << "Buy products" << std::endl;
	}
}