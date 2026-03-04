#include "BaseWalletHolder.hpp"

BaseWalletHolder::BaseWalletHolder(const Money initialAmount)
	: m_wallet(initialAmount)
{
}

IMoneyStorage& BaseWalletHolder::GetWallet()
{
	return m_wallet;
}