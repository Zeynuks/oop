#pragma once

#include "Bank.hpp"

class MoneyTransfer
{
public:
	MoneyTransfer(IMoneyStorage& from, Money amount);
	~MoneyTransfer();

	void To(IMoneyStorage& to);

private:
	IMoneyStorage& m_from;
	Money m_amount;
	bool m_committed = false;
};