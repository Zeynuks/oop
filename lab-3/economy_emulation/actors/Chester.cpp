#include "Chester.hpp"

#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"
#include "Liza.hpp"
#include "Random.hpp"

Chester::Chester(const ActorId id, Bank& bank, const ActorId homerId, const ActorId apuId)
	: BaseActor(id, "Chester", bank)
	, m_homerId(homerId)
	, m_apuId(apuId)
{
	IMoneyStorage& account = m_bank.OpenAccount();
	m_bankAccount = std::ref(account);
}

void Chester::Tick(EconomyContext& context)
{
	if (RandomBool())
	{
		IMoneyStorage& account = GetBankAccount();

		IActor& homer = context.GetActor(m_homerId);
		IMoneyStorage& homerBankAccount = homer.StealMoney();

		ReceiveBankTransfer(homerBankAccount, RandomMoney(100, 150));

		IActor& apu = context.GetActor(m_apuId);
		apu.ReceiveCash(account, 100);
	}
}