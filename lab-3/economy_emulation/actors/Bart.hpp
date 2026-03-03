#pragma once

#include "../core/BaseActor.hpp"

class Bart : public BaseActor
{
public:
	explicit Bart(ActorId id, Bank& bank, ActorId apuId);
	void Tick(EconomyContext& context) override;
	IMoneyStorage& StealMoney() override;

private:
	ActorId m_apuId;
};