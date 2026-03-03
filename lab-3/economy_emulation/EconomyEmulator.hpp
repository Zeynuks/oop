#pragma once
#include "core/EconomyContext.hpp"

class EconomyEmulator
{
public:
	void InitEmulator();
	void Emulate(int iterations);

private:
	Bank m_bank;
	EconomyContext m_context;

	void ActorIteration(IActor& actor);
};