#pragma once

#include "Bank.hpp"
#include "IActor.hpp"
#include "IMoneyReceiver.hpp"
#include "ISimulatable.hpp"
#include "BaseActor.hpp"
#include "BaseBankAccountHolder.hpp"
#include "BaseWalletHolder.hpp"

class Person : public ISimulatable
	, public IMoneyReceiver
	, public BaseActor
	, public BaseBankAccountHolder
	, BaseWalletHolder
{
public:
	explicit Person(ActorId id, const std::string& name, Bank& bank);
	void ReceiveBankTransfer(IMoneyStorage& from, Money amount) override;
	void ReceiveCash(IMoneyStorage& from, Money amount) override;

	IMoneyStorage& GetBankAccount() const override;
	IMoneyStorage& GetWallet() override;
};
