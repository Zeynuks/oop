#pragma once
#include "../core/BaseActor.hpp"

class Berns : public BaseActor
{
public:
	explicit Berns(ActorId id, Bank& bank, ActorId homerId);
	void Tick(EconomyContext& context) override;

private:
	ActorId m_homerId;
};