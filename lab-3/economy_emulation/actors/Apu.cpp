#include "Apu.hpp"
#include "EconomyContext.hpp"

#include <iostream>

Apu::Apu(const ActorId id, Bank& bank, const ActorId bernsId)
	: Person(id, "Apu", bank)
	, m_bernsId(bernsId)
{
	BaseBankAccountHolder::OpenBankAccount();
}

void Apu::Tick(EconomyContext& context)
{
	IMoneyStorage& account = GetBankAccount();
	auto& berns = context.GetActor<IMoneyReceiver>(m_bernsId);

	ReceiveBankTransfer(account, GetWallet().GetBalance());
	std::cout << GetName() << ": " << "Transfer money on BankAccount" << std::endl;

	berns.ReceiveBankTransfer(account, 80);
	std::cout << GetName() << ": " << "Paid for eclecticism" << std::endl;
}