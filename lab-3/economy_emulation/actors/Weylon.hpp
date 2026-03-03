#pragma once
#include "../core/BaseActor.hpp"

class Weylon : public BaseActor
{
public:
	explicit Weylon(ActorId id, Bank& bank, ActorId apuId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_apuId;
};