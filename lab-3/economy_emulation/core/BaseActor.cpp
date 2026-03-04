#include "BaseActor.hpp"

BaseActor::BaseActor(const ActorId id, const std::string& name)
	: m_id(id)
	, m_name(name)
{
}

ActorId BaseActor::GetId() const
{
	return m_id;
}

std::string BaseActor::GetName() const
{
	return m_name;
}