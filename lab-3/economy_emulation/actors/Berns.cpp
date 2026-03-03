#include "Berns.hpp"

#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"

#include <iostream>

Berns::Berns(const ActorId id, Bank& bank, const ActorId homerId)
	: BaseActor(id, "Berns", bank)
	, m_homerId(homerId)
{
	IMoneyStorage& account = m_bank.OpenAccount(2000);
	m_bankAccount = std::ref(account);
}

void Berns::Tick(EconomyContext& context)
{
	IMoneyStorage& account = GetBankAccount();

	IActor& homer = context.GetActor(m_homerId);
	homer.ReceiveBankTransfer(account, 200);
	std::cout << GetName() << ": " << "gave Homer a salary" << std::endl;
}