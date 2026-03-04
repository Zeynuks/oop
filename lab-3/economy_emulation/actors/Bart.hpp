#pragma once

#include "BaseActor.hpp"
#include "IStealable.hpp"

class Bart : public BaseActor, public IStealable
{
public:
	explicit Bart(ActorId id, Bank& bank, ActorId apuId);
	void Tick(EconomyContext& context) override;
	void StealMoney(IFinancialActor& stealer, Money amount) override;

private:
	ActorId m_apuId;
};