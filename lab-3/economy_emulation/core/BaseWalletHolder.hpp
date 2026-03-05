#pragma once

#include "IWalletHolder.hpp"
#include "money_storage/Wallet.hpp"

class BaseWalletHolder : IWalletHolder
{
public:
	explicit BaseWalletHolder(const Money initialAmount)
		: m_wallet(initialAmount)
	{
	}

	IMoneyStorage& GetWallet() override
	{
		return m_wallet;
	}

private:
	Wallet m_wallet;
};