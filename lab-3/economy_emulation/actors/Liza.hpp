#pragma once

#include "../core/Person.hpp"

class Liza : public Person
{
public:
	explicit Liza(ActorId id, Bank& bank, ActorId apuId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_apuId;
};