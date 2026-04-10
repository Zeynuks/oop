#pragma once

#include "ActorBuilder.hpp"
#include "core/Bank.hpp"
#include "core/EconomyContext.hpp"
#include "core/ISimulatable.hpp"

#include <iostream>

class EconomyEmulator
{
public:
	void InitEmulator()
	{
		ActorBuilder builder(m_bank, m_bankAccount);

		m_context.AddActor(builder.BuildHomer());
		m_context.AddActor(builder.BuildMarge());
		m_context.AddActor(builder.BuildBart());
		m_context.AddActor(builder.BuildLiza());
		m_context.AddActor(builder.BuildApu());
		m_context.AddActor(builder.BuildBerns());
		m_context.AddActor(builder.BuildNelson());
		m_context.AddActor(builder.BuildWeylon());
		m_context.AddActor(builder.BuildChester());
	}

	void Emulate(const int iterations)
	{
		for (int i = 0; i < iterations; ++i)
		{
			std::cout << "Iteration: " << i + 1 << std::endl;

			for (auto actor : m_context.GetAllActors())
			{
				ActorIteration(actor.get());
			}
		}
	}

private:
	BankAccount m_bankAccount = BankAccount(20000);
	Bank m_bank = Bank(m_bankAccount);
	EconomyContext m_context;

	void ActorIteration(const IActor& actor)
	{
		try
		{
			auto& simulatable = m_context.GetActor<ISimulatable>(actor.GetId());
			simulatable.Tick(m_context);
		}
		catch (const std::runtime_error& e)
		{
			std::cout << actor.GetName() << ": " << e.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "Undefined error!" << std::endl;
		}
	}
};