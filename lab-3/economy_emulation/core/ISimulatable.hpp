#pragma once

class EconomyContext;

class ISimulatable
{
public:
	virtual ~ISimulatable() = default;
	virtual void Tick(EconomyContext& context) = 0;
};