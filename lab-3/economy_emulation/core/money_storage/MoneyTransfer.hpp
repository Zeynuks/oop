#pragma once

#include "../Bank.hpp"

#include <stdexcept>

class MoneyTransfer
{
public:
	MoneyTransfer(IMoneyStorage& from, const Money amount)
		: m_from(from)
	{
		m_amount = m_from.Withdraw(amount);
	}

	void To(IMoneyStorage& to)
	{
		if (m_committed)
		{
			throw std::logic_error("Transfer already committed");
		}

		to.Deposit(m_amount);
		m_committed = true;
	}

	~MoneyTransfer()
	{
		if (!m_committed)
		{
			m_from.Deposit(m_amount);
		}
	}

private:
	IMoneyStorage& m_from;
	Money m_amount;
	bool m_committed = false;
};