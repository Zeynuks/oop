#include "Weylon.hpp"

#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"
#include "Random.hpp"

Weylon::Weylon(const ActorId id, Bank& bank, const ActorId apuId)
	: BaseActor(id, "Waylon", bank)
	, m_apuId(apuId)
{
	IMoneyStorage& account = m_bank.OpenAccount();
	m_bankAccount = std::ref(account);
}

void Weylon::Tick(EconomyContext& context)
{
	if (RandomBool() || m_bankAccount)
	{
		ReceiveCash(m_wallet, m_bank.CloseAccount(GetBankAccount()));

		IActor& apu = context.GetActor(m_apuId);
		apu.ReceiveBankTransfer(m_wallet, 75);
	} else if (m_bankAccount)
	{
		IMoneyStorage& account = GetBankAccount();

		IActor& apu = context.GetActor(m_apuId);
		apu.ReceiveBankTransfer(account, 75);
	} else
	{
		IMoneyStorage& account = m_bank.OpenAccount();
		m_bankAccount = std::ref(account);

		IActor& apu = context.GetActor(m_apuId);
		apu.ReceiveBankTransfer(account, 75);
	}
}