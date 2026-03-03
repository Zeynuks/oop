#pragma once
#include "IActor.hpp"

#include <memory>
#include <unordered_map>

class EconomyContext
{
public:
	void AddActor(std::unique_ptr<IActor> actor);
	[[nodiscard]] IActor& GetActor(ActorId id);
	[[nodiscard]] std::list<std::reference_wrapper<IActor>> GetAllActors();

private:
	std::unordered_map<ActorId, std::unique_ptr<IActor>> m_actors;
};