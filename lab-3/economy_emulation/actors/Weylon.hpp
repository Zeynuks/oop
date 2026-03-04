#pragma once

#include "../core/Person.hpp"

class Weylon : public Person
{
public:
	explicit Weylon(ActorId id, Bank& bank, ActorId apuId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_apuId;
};