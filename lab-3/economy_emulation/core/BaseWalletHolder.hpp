#pragma once

#include "IWalletHolder.hpp"
#include "money_storage/Wallet.hpp"

class BaseWalletHolder: IWalletHolder
{
public:
	explicit BaseWalletHolder(Money initialAmount = 0);
	IMoneyStorage& GetWallet() override;

private:
	Wallet m_wallet;
};