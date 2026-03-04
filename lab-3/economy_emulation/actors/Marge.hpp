#pragma once

#include "../core/Person.hpp"

class Marge : public Person
{
public:
	explicit Marge(ActorId id, Bank& bank, ActorId apuId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_apuId;
};