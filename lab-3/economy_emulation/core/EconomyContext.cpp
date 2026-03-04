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

std::vector<std::reference_wrapper<IActor>> EconomyContext::GetAllActors()
{
	std::vector<std::reference_wrapper<IActor>> result;

	for (auto& actor : m_actors | std::views::values)
	{
		result.push_back(*actor);
	}

	return result;
}