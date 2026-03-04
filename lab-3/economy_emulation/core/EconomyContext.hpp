#pragma once
#include "IActor.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

class EconomyContext
{
public:
	void AddActor(std::unique_ptr<IActor> actor);
	[[nodiscard]] std::vector<std::reference_wrapper<IActor>> GetAllActors();

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