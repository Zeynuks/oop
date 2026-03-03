#include "Berns.hpp"

#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"

#include <iostream>

Berns::Berns(const ActorId id, Bank& bank, const ActorId homerId, const ActorId weylonId)
	: BaseActor(id, "Berns", bank)
	, m_homerId(homerId)
	, m_weylonId(weylonId)
{
	BaseActor::OpenBankAccount(2000);
}

void Berns::Tick(EconomyContext& context)
{
	IMoneyStorage& account = GetBankAccount();

	IActor& homer = context.GetActor(m_homerId);
	homer.ReceiveBankTransfer(account, 200);
	std::cout << GetName() << ": " << "Gave Homer a salary" << std::endl;

	IActor& weylon = context.GetActor(m_weylonId);

	weylon.ReceiveBankTransfer(account, 200);
	std::cout << GetName() << ": " << "Gave Weylon a salary" << std::endl;
}