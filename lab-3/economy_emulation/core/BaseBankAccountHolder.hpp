#pragma once

#include "Bank.hpp"
#include "IBankAccountHolder.hpp"
#include "money_storage/IMoneyStorage.hpp"

#include <optional>
#include <functional>

class BaseBankAccountHolder : IBankAccountHolder
{
public:
	explicit BaseBankAccountHolder(Bank& m_bank);
	IMoneyStorage& GetBankAccount() const override;
	void OpenBankAccount() override;
	void CloseBankAccount(IMoneyStorage& to) override;

private:
	Bank& m_bank;
	std::optional<std::reference_wrapper<IMoneyStorage>> m_bankAccount = std::nullopt;
};
