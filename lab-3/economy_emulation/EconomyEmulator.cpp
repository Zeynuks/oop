#include "EconomyEmulator.hpp"
#include "ActorBuilder.hpp"

#include <iostream>
#include <ostream>

void EconomyEmulator::InitEmulator()
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

void EconomyEmulator::Emulate(const int iterations)
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

void EconomyEmulator::ActorIteration(const IActor& actor)
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