#include <gtest/gtest.h>

#include "BaseBankAccountHolder.hpp"
#include "money_storage/BaseMoneyStorage.hpp"
#include "money_storage/MoneyTransfer.hpp"

#include <memory>

class TestBank
{
public:
	void OpenAccount(BankAccount& account)
	{
		m_account.emplace(account);
	}

	BankAccount& GetBankAccount()
	{
		return m_account.value();
	}

	void CloseAccount(IMoneyStorage&)
	{
		m_account = std::nullopt;
	}

private:
	std::optional<BankAccount> m_account = std::nullopt;
};

TEST(BaseBankAccountHolderTests, OpenBankAccountCreatesAccount)
{
	TestBank bank;
	auto account = BankAccount();

	bank.OpenAccount(account);

	EXPECT_NO_THROW(bank.GetBankAccount());
}

TEST(BaseBankAccountHolderTests, CloseBankAccountTransfersMoney)
{
	TestBank bank;
	BaseMoneyStorage from(50);

	auto account = BankAccount();

	bank.OpenAccount(account);
	IMoneyStorage& storage = bank.GetBankAccount();

	MoneyTransfer(from, from.GetBalance()).To(storage);

	BaseMoneyStorage target(100);

	bank.CloseAccount(target);

	EXPECT_EQ(target.GetBalance(), 100);
}