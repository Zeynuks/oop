#pragma once

#include "BaseActor.hpp"
#include "EconomyContext.hpp"
#include "IStealable.hpp"

class Homer : public BaseActor
	, public IStealable
{
public:
	explicit Homer(ActorId id, Bank& bank, ActorId margeId, ActorId bernsId, ActorId bartId, ActorId lisaId, ActorId chesterId);
	void Tick(EconomyContext& context) override;
	IMoneyStorage& GetStorageToSteal() override;

private:
	ActorId m_margeId;
	ActorId m_bernsId;
	ActorId m_bartId;
	ActorId m_lizaId;
	ActorId m_chesterId;
};