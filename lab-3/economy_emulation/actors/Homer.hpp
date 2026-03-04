#pragma once

#include "../core/IStealable.hpp"
#include "../core/Person.hpp"

class Homer : public Person
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