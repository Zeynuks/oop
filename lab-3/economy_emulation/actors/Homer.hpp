#pragma once

#include "../core/EconomyContext.hpp"
#include "../core/IStealable.hpp"
#include "../core/Person.hpp"

class Homer : public Person
	, public IStealable
{
public:
	Homer(const ActorId id, Bank& bank, const ActorId margeId, const ActorId bernsId, const ActorId bartId, const ActorId lisaId, const ActorId chesterId)
		: Person(id, "Homer", bank)
		, m_margeId(margeId)
		, m_bernsId(bernsId)
		, m_bartId(bartId)
		, m_lizaId(lisaId)
		, m_chesterId(chesterId)
	{
		Person::OpenBankAccount();
	}

	void Tick(EconomyContext& context) override
	{
		IMoneyStorage& account = GetBankAccount();

		auto& marge = context.GetActor<IMoneyReceiver>(m_margeId);
		marge.ReceiveBankTransfer(account, RandomMoney(50, 100));
		std::cout << GetName() << ": " << "Send money to Marge" << std::endl;

		auto& berns = context.GetActor<IMoneyReceiver>(m_bernsId);
		berns.ReceiveBankTransfer(account, RandomMoney(20, 75));
		std::cout << GetName() << ": " << "Paid for eclecticism" << std::endl;

		auto& liza = context.GetActor<IMoneyReceiver>(m_lizaId);
		liza.ReceiveCash(account, RandomMoney(0, 50));
		std::cout << GetName() << ": " << "Get money to Liza" << std::endl;

		auto& bart = context.GetActor<IMoneyReceiver>(m_bartId);
		bart.ReceiveCash(account, RandomMoney(0, 50));
		std::cout << GetName() << ": " << "Get money to Bart" << std::endl;
	}

	IMoneyStorage& GetStorageToSteal() override
	{
		return GetBankAccount();
	}

private:
	ActorId m_margeId;
	ActorId m_bernsId;
	ActorId m_bartId;
	ActorId m_lizaId;
	ActorId m_chesterId;
};