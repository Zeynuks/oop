#pragma once
#include "IActor.hpp"

class BaseActor : public IActor
{
public:
	BaseActor(ActorId id, const std::string& name);
	ActorId GetId() const override;
	std::string GetName() const override;

private:
	ActorId m_id;
	std::string m_name;
};