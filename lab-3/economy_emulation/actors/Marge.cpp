#include "Marge.hpp"
#include "EconomyContext.hpp"

#include <iostream>

Marge::Marge(const ActorId id, Bank& bank, const ActorId apuId)
	: Person(id, "Marge", bank)
	, m_apuId(apuId)
{
	Person::OpenBankAccount();
}

void Marge::Tick(EconomyContext& context)
{
	IMoneyStorage& account = GetBankAccount();

	auto& apu = context.GetActor<IMoneyReceiver>(m_apuId);
	apu.ReceiveBankTransfer(account, 75);
	std::cout << GetName() << ": " << "Buy products" << std::endl;
}