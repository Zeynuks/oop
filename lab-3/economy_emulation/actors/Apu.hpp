#pragma once

#include "../core/BaseActor.hpp"

class Apu : public BaseActor
{
public:
	explicit Apu(ActorId id, Bank& bank, ActorId bernsId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_bernsId;
};