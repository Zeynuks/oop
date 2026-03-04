#pragma once

#include "Bank.hpp"
#include "IActor.hpp"
#include "IFinancialActor.hpp"
#include "ISimulatable.hpp"
#include "money_storage/Wallet.hpp"

#include <functional>
#include <optional>

class Person : public IActor
	, public ISimulatable
	, public IFinancialActor
{
public:
	explicit Person(ActorId id, const std::string& name, Bank& bank, Money initialWalletCash = 0);
	ActorId GetId() const override;
	std::string GetName() const override;

	void Tick(EconomyContext& context) override = 0;

	bool HasBankAccount() override;
	void ReceiveBankTransfer(IMoneyStorage& from, Money amount) override;
	void ReceiveCash(IMoneyStorage& from, Money amount) override;

private:
	ActorId m_id;
	std::string m_name;
	Bank& m_bank;

protected:
	Wallet m_wallet;
	std::optional<std::reference_wrapper<IMoneyStorage>> m_bankAccount;

	IMoneyStorage& GetBankAccount() const;
	void OpenBankAccount(Money initialAmount = 0) override;
	void CloseBankAccount() override;
};
