#include "Homer.hpp"

#include "../core/EconomyContext.hpp"
#include "../core/MoneyTransfer.hpp"
#include "Random.hpp"

Homer::Homer(const ActorId id, Bank& bank, const ActorId margeId, const ActorId bernsId, const ActorId bartId, const ActorId lisaId, const ActorId chesterId)
	: BaseActor(id, "Homer", bank)
	, m_margeId(margeId)
	, m_bernsId(bernsId)
	, m_bartId(bartId)
	, m_lizaId(lisaId)
	, m_chesterId(chesterId)
{
	IMoneyStorage& account = m_bank.OpenAccount();
	m_bankAccount = std::ref(account);
}

void Homer::Tick(EconomyContext& context)
{
	IMoneyStorage& account = GetBankAccount();

	IActor& marge = context.GetActor(m_margeId);
	IActor& berns = context.GetActor(m_bernsId);
	IActor& liza = context.GetActor(m_lizaId);
	IActor& bart = context.GetActor(m_bartId);

	marge.ReceiveBankTransfer(account, RandomMoney(50, 100));
	berns.ReceiveBankTransfer(account, RandomMoney(20, 75));
	liza.ReceiveBankTransfer(account, RandomMoney(0, 50));
	bart.ReceiveBankTransfer(account, RandomMoney(0, 50));
}

IMoneyStorage& Homer::StealMoney()
{
	return GetBankAccount();
}