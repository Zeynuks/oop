#pragma once

#include "IActor.hpp"
#include "Wallet.hpp"

#include <functional>
#include <optional>

class BaseActor : public IActor
{
public:
	explicit BaseActor(ActorId id, const std::string& name, Bank& bank);
	ActorId GetId() const override;
	std::string GetName() const override;
	void ReceiveBankTransfer(IMoneyStorage& from, Money amount) override;
	void ReceiveCash(IMoneyStorage& from, Money amount) override;
	IMoneyStorage& StealMoney() override;

private:
	ActorId m_id;
	std::string m_name;

	void WithdrawMoney(Money amount) override;
	void DepositMoney(Money amount) override;

protected:
	Bank& m_bank;
	Wallet m_wallet;
	std::optional<std::reference_wrapper<IMoneyStorage>> m_bankAccount;

	IMoneyStorage& GetBankAccount() override;
};
