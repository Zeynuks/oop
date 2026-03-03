#include "Marge.hpp"

#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"

#include <iostream>

Marge::Marge(const ActorId id, Bank& bank, const ActorId apuId)
	: BaseActor(id, "Marge", bank)
	, m_apuId(apuId)
{
	BaseActor::OpenBankAccount();
}

void Marge::Tick(EconomyContext& context)
{
	IMoneyStorage& account = GetBankAccount();

	IActor& apu = context.GetActor(m_apuId);

	apu.ReceiveBankTransfer(account, 75);
	std::cout << GetName() << ": " << "Buy products" << std::endl;
}