#pragma once

#include "Bank.hpp"
#include "BaseActor.hpp"
#include "BaseBankAccountHolder.hpp"
#include "BaseWalletHolder.hpp"
#include "IActor.hpp"
#include "IMoneyReceiver.hpp"
#include "ISimulatable.hpp"
#include "money_storage/MoneyTransfer.hpp"

class Person : public ISimulatable
	, public IMoneyReceiver
	, public BaseActor
	, public BaseBankAccountHolder
	, BaseWalletHolder
{
public:
	Person(const ActorId id, const std::string& name, Bank& bank)
		: BaseActor(id, name)
		, BaseBankAccountHolder(bank)
		, BaseWalletHolder(0)
	{
	}

	void ReceiveBankTransfer(IMoneyStorage& from, const Money amount) override
	{
		IMoneyStorage& account = GetBankAccount();
		MoneyTransfer(from, amount).To(account);
	}

	void ReceiveCash(IMoneyStorage& from, const Money amount) override
	{
		MoneyTransfer(from, amount).To(GetWallet());
	}

	IMoneyStorage& GetBankAccount() const override
	{
		return BaseBankAccountHolder::GetBankAccount();
	}

	IMoneyStorage& GetWallet() override
	{
		return BaseWalletHolder::GetWallet();
	}
};
