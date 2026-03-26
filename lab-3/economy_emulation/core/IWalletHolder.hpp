#pragma once

#include "money_storage/Wallet.hpp"

class IWalletHolder
{
public:
	virtual ~IWalletHolder() = default;
	virtual IMoneyStorage& GetWallet() = 0;
};