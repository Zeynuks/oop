#pragma once

#include "../core/IStealable.hpp"
#include "../core/Person.hpp"

class Bart : public Person, public IStealable
{
public:
	explicit Bart(ActorId id, Bank& bank, ActorId apuId);
	void Tick(EconomyContext& context) override;
	IMoneyStorage& GetStorageToSteal() override;

private:
	ActorId m_apuId;
};