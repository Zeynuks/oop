#pragma once
#include "IActor.hpp"

#include <memory>
#include <ranges>
#include <stdexcept>
#include <unordered_map>
#include <vector>

class EconomyContext
{
public:
	void AddActor(std::unique_ptr<IActor> actor)
	{
		if (actor)
		{
			const ActorId id = actor->GetId();
			m_actors[id] = std::move(actor);
		}
	}

	std::vector<std::reference_wrapper<IActor>> GetAllActors()
	{
		std::vector<std::reference_wrapper<IActor>> result;

		for (auto& actor : m_actors | std::views::values)
		{
			result.push_back(*actor);
		}

		return result;
	}

	template <typename T>
	T& GetActor(const ActorId id)
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

private:
	std::unordered_map<ActorId, std::unique_ptr<IActor>> m_actors;
};