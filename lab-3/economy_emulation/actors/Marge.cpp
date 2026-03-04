#include "Marge.hpp"

#include "EconomyContext.hpp"
#include "MoneyTransfer.hpp"

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

	auto& apu = context.GetActor<IFinancialActor>(m_apuId);
	apu.ReceiveBankTransfer(account, 75);
	std::cout << GetName() << ": " << "Buy products" << std::endl;
}