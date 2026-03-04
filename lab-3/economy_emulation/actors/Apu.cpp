#include "Apu.hpp"
#include "EconomyContext.hpp"

#include <iostream>

Apu::Apu(const ActorId id, Bank& bank, const ActorId bernsId)
	: Person(id, "Apu", bank, 200)
	, m_bernsId(bernsId)
{
	Person::OpenBankAccount(200);
}

void Apu::Tick(EconomyContext& context)
{
	IMoneyStorage& account = GetBankAccount();
	auto& berns = context.GetActor<IFinancialActor>(m_bernsId);

	ReceiveBankTransfer(account, m_wallet.GetBalance());
	std::cout << GetName() << ": " << "Transfer money on BankAccount" << std::endl;

	berns.ReceiveBankTransfer(account, 80);
	std::cout << GetName() << ": " << "Paid for eclecticism" << std::endl;
}