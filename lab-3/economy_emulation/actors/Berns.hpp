#pragma once
#include "BaseActor.hpp"

class Berns : public BaseActor
{
public:
	explicit Berns(ActorId id, Bank& bank, ActorId homerId, ActorId weylonId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_homerId;
	ActorId m_weylonId;
};