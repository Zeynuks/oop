#include "MoneyTransfer.hpp"

#include <stdexcept>

MoneyTransfer::MoneyTransfer(IMoneyStorage& from, const Money amount)
	: m_from(from)
{
	m_amount = m_from.Withdraw(amount);
}

void MoneyTransfer::To(IMoneyStorage& to)
{
	if (m_committed)
	{
		throw std::logic_error("Transfer already committed");
	}

	to.Deposit(m_amount);
	m_committed = true;
}

MoneyTransfer::~MoneyTransfer()
{
	if (!m_committed)
	{
		m_from.Deposit(m_amount);
	}
}