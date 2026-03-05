#pragma once

#include "Bank.hpp"
#include "IBankAccountHolder.hpp"
#include "errors/BankAccountError.hpp"
#include "money_storage/IMoneyStorage.hpp"
#include "money_storage/MoneyTransfer.hpp"

#include <functional>
#include <optional>

class BaseBankAccountHolder : IBankAccountHolder
{
public:
	explicit BaseBankAccountHolder(Bank& m_bank)
		: m_bank(m_bank)
	{
	}

	IMoneyStorage& GetBankAccount() const override
	{
		if (!m_bankAccount)
		{
			throw BankAccountError("No bank account");
		}

		return m_bankAccount->get();
	}

	void OpenBankAccount() override
	{
		if (!m_bankAccount.has_value())
		{
			IMoneyStorage& account = m_bank.OpenAccount();
			m_bankAccount = std::ref(account);
		}
	}

	void CloseBankAccount(IMoneyStorage& to) override
	{
		IMoneyStorage& account = GetBankAccount();
		MoneyTransfer(account, account.GetBalance()).To(to);
		m_bank.CloseAccount(account);

		m_bankAccount = std::nullopt;
	}

private:
	Bank& m_bank;
	std::optional<std::reference_wrapper<IMoneyStorage>> m_bankAccount = std::nullopt;
};
