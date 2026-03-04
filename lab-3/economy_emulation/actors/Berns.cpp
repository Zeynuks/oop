#include "Berns.hpp"

#include "EconomyContext.hpp"

#include <iostream>

Berns::Berns(const ActorId id, Bank& bank, const ActorId homerId, const ActorId weylonId)
	: Person(id, "Berns", bank)
	, m_homerId(homerId)
	, m_weylonId(weylonId)
{
	Person::OpenBankAccount();
}

void Berns::Tick(EconomyContext& context)
{
	IMoneyStorage& account = GetBankAccount();

	auto& homer = context.GetActor<IMoneyReceiver>(m_homerId);
	homer.ReceiveBankTransfer(account, 200);
	std::cout << GetName() << ": " << "Gave Homer a salary" << std::endl;

	auto& weylon = context.GetActor<IMoneyReceiver>(m_weylonId);
	weylon.ReceiveBankTransfer(account, 200);
	std::cout << GetName() << ": " << "Gave Weylon a salary" << std::endl;
}