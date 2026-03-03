#pragma once

#include "Bank.hpp"

class EconomyContext;

using ActorId = unsigned long long;

class IMoneyStorage;

class IActor
{
public:
	virtual void Tick(EconomyContext& context) = 0;
	virtual ActorId GetId() const = 0;
	virtual std::string GetName() const = 0;
	virtual IMoneyStorage& GetBankAccount() = 0;
	virtual void OpenBankAccount() = 0;
	virtual void OpenBankAccount(Money amount) = 0;
	virtual void CloseBankAccount() = 0;
	virtual void ReceiveBankTransfer(IMoneyStorage& from, Money amount) = 0;
	virtual void ReceiveCash(IMoneyStorage& from, Money amount) = 0;
	virtual IMoneyStorage& StealMoney() = 0;
	virtual ~IActor() = default;

private:
	virtual void WithdrawMoney(Money amount) = 0;
	virtual void DepositMoney(Money amount) = 0;
};
