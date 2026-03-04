#pragma once

#include "core/Bank.hpp"
#include "core/EconomyContext.hpp"

class EconomyEmulator
{
public:
	void InitEmulator();
	void Emulate(int iterations);

private:
	BankAccount m_bankAccount = BankAccount(20000);
	Bank m_bank = Bank(m_bankAccount);
	EconomyContext m_context;

	void ActorIteration(const IActor& actor);
};