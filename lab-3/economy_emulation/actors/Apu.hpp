#pragma once

#include "../core/Person.hpp"

class Apu : public Person
{
public:
	explicit Apu(ActorId id, Bank& bank, ActorId bernsId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_bernsId;
};