#pragma once

#include "Bank.hpp"
#include "IActor.hpp"
#include "IFinancialActor.hpp"
#include "ISimulatable.hpp"
#include "Wallet.hpp"

#include <functional>
#include <optional>

class BaseActor : public IActor
	, public ISimulatable
	, public IFinancialActor
{
public:
	explicit BaseActor(ActorId id, const std::string& name, Bank& bank, Money initialWalletCash = 0);
	ActorId GetId() const override;
	std::string GetName() const override;
	void ReceiveBankTransfer(IMoneyStorage& from, Money amount) override;
	void ReceiveCash(IMoneyStorage& from, Money amount) override;

private:
	ActorId m_id;
	std::string m_name;

protected:
	Bank& m_bank;
	Wallet m_wallet;
	std::optional<std::reference_wrapper<IMoneyStorage>> m_bankAccount;

	IMoneyStorage& GetBankAccount() override;
	void OpenBankAccount(Money initialAmount = 0) override = 0;
	void CloseBankAccount() override = 0;
};
