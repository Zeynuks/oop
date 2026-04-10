#include <gtest/gtest.h>

#include "../core/money_storage/BaseMoneyStorage.hpp"
#include "../core/money_storage/MoneyTransfer.hpp"
#include "../core/errors/MoneyStorageError.hpp"

TEST(BaseMoneyStorageTests, ConstructorSetsInitialBalance)
{
	const BaseMoneyStorage storage(100);

	EXPECT_EQ(storage.GetBalance(), 100);
}

TEST(BaseMoneyStorageTests, IdsAreUnique)
{
	const BaseMoneyStorage a(0);
	const BaseMoneyStorage b(0);

	EXPECT_NE(a.GetId(), b.GetId());
}

TEST(BaseMoneyStorageTests, TransferDepositsMoney)
{
	BaseMoneyStorage from(100);
	BaseMoneyStorage to(0);

	MoneyTransfer(from, 40).To(to);

	EXPECT_EQ(from.GetBalance(), 60);
	EXPECT_EQ(to.GetBalance(), 40);
}

TEST(BaseMoneyStorageTests, TransferAllMoney)
{
	BaseMoneyStorage from(50);
	BaseMoneyStorage to(0);

	MoneyTransfer(from, from.GetBalance()).To(to);

	EXPECT_EQ(from.GetBalance(), 0);
	EXPECT_EQ(to.GetBalance(), 50);
}

TEST(BaseMoneyStorageTests, WithdrawTooMuchThrows)
{
	BaseMoneyStorage from(20);
	BaseMoneyStorage to(0);

	EXPECT_THROW(
		MoneyTransfer(from, 30).To(to),
		MoneyStorageError
	);
}

TEST(BaseMoneyStorageTests, WithdrawNegativeThrows)
{
	BaseMoneyStorage from(20);
	BaseMoneyStorage to(0);

	EXPECT_THROW(
		MoneyTransfer(from, -10).To(to),
		MoneyStorageError
	);
}

TEST(BaseMoneyStorageTests, DepositOverflowThrows)
{
	BaseMoneyStorage from(1);
	BaseMoneyStorage to(std::numeric_limits<Money>::max());

	EXPECT_THROW(
		MoneyTransfer(from, 1).To(to),
		MoneyStorageError
	);
}