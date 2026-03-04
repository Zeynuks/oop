#pragma once

using Money = long long;
using StorageId = unsigned long long;

class MoneyTransfer;

class IMoneyStorage
{

public:
	virtual ~IMoneyStorage() = default;
	virtual StorageId GetId() const = 0;
	virtual Money GetBalance() const = 0;

private:
	friend class MoneyTransfer;

	virtual void Deposit(Money amount) = 0;
	virtual Money Withdraw(Money amount) = 0;
};