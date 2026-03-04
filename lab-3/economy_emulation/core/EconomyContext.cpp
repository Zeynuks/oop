#include "EconomyContext.hpp"

#include <ranges>

void EconomyContext::AddActor(std::unique_ptr<IActor> actor)
{
	if (actor)
	{
		const ActorId id = actor->GetId();
		m_actors[id] = std::move(actor);
	}
}

template <typename T>
T& EconomyContext::GetActor(const ActorId id)
{
	if (const auto it = m_actors.find(id); it != m_actors.end())
	{
		if (auto* casted = dynamic_cast<T*>(it->second.get()))
		{
			return *casted;
		}
		throw std::runtime_error("Actor found, but requested interface is not supported.");
	}

	throw std::runtime_error("Actor with specified ID not found.");
}

std::list<std::reference_wrapper<IActor>> EconomyContext::GetAllActors()
{
	std::list<std::reference_wrapper<IActor>> result;

	for (auto& actor : m_actors | std::views::values)
	{
		result.push_back(*actor);
	}

	return result;
}