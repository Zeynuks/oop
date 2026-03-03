#include "Apu.hpp"
#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"

Apu::Apu(const ActorId id, Bank& bank, const ActorId bernsId)
	: BaseActor(id, "Apu", bank)
	, m_bernsId(bernsId)
{
	IMoneyStorage& account = m_bank.OpenAccount(200);
	m_bankAccount = std::ref(account);
}

void Apu::Tick(EconomyContext& context)
{
	IMoneyStorage& account = GetBankAccount();
	IActor& berns = context.GetActor(m_bernsId);

	ReceiveBankTransfer(account, m_wallet.GetBalance());
	berns.ReceiveBankTransfer(account, 80);
}