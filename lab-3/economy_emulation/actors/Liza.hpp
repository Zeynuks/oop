#pragma once

#include "BaseActor.hpp"

class Liza : public BaseActor
{
public:
	explicit Liza(ActorId id, Bank& bank, ActorId apuId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_apuId;
};