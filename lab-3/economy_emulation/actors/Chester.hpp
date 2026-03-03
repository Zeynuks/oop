#pragma once

#include "../core/BaseActor.hpp"

class Chester : public BaseActor
{
public:
	explicit Chester(ActorId id, Bank& bank, ActorId homerId, ActorId apuId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_homerId;
	ActorId m_apuId;
};