#pragma once

#include "../core/Person.hpp"

class Nelson : public Person
{
public:
	explicit Nelson(ActorId id, Bank& bank, ActorId bartId, ActorId apuId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_bartId;
	ActorId m_apuId;
};