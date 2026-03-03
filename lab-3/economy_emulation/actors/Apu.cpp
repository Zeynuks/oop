#include "Apu.hpp"
#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"

#include <iostream>

Apu::Apu(const ActorId id, Bank& bank, const ActorId bernsId)
	: BaseActor(id, "Apu", bank)
	, m_bernsId(bernsId)
{
	BaseActor::OpenBankAccount(200);
}

void Apu::Tick(EconomyContext& context)
{
	IMoneyStorage& account = GetBankAccount();
	IActor& berns = context.GetActor(m_bernsId);

	ReceiveBankTransfer(account, m_wallet.GetBalance());
	std::cout << GetName() << ": " << "Transfer money on BankAccount" << std::endl;

	berns.ReceiveBankTransfer(account, 80);
	std::cout << GetName() << ": " << "Paid for eclecticism" << std::endl;
}