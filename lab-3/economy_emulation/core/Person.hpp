#pragma once

#include "Bank.hpp"
#include "IActor.hpp"
#include "IBankAccountHolder.hpp"
#include "IMoneyReceiver.hpp"
#include "ISimulatable.hpp"
#include "money_storage/Wallet.hpp"

#include <functional>
#include <optional>

class Person : public IActor
	, public ISimulatable
	, public IMoneyReceiver
	, IBankAccountHolder
{
public:
	explicit Person(ActorId id, const std::string& name, Bank& bank, Money initialWalletCash = 0);
	ActorId GetId() const override;
	std::string GetName() const override;

	void Tick(EconomyContext& context) override = 0;

	void ReceiveBankTransfer(IMoneyStorage& from, Money amount) override;
	void ReceiveCash(IMoneyStorage& from, Money amount) override;

	IMoneyStorage& GetBankAccount() const override;
	void OpenBankAccount(Money initialAmount = 0) override;
	void CloseBankAccount() override;

private:
	ActorId m_id;
	std::string m_name;
	Bank& m_bank;

protected:
	Wallet m_wallet;
	std::optional<std::reference_wrapper<IMoneyStorage>> m_bankAccount;
};
