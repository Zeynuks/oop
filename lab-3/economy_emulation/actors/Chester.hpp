#pragma once

#include "../core/Person.hpp"

class Chester : public Person
{
public:
	explicit Chester(ActorId id, Bank& bank, ActorId homerId, ActorId apuId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_homerId;
	ActorId m_apuId;
};