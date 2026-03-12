#pragma once

#include "IActor.hpp"

class BaseActor : public IActor
{
public:
	BaseActor(const ActorId id, const std::string& name)
		: m_id(id)
		, m_name(name)
	{
	}

	ActorId GetId() const override
	{
		return m_id;
	}

	std::string GetName() const override
	{
		return m_name;
	}

private:
	ActorId m_id;
	std::string m_name;
};