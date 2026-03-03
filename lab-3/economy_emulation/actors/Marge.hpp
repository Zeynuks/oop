#pragma once
#include "../core/BaseActor.hpp"

class Marge : public BaseActor
{
public:
	explicit Marge(ActorId id, Bank& bank, ActorId apuId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_apuId;
};