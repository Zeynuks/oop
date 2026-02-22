#pragma once

#include "../Car.hpp"
#include "ICommand.hpp"

class TurnOnEngineCommand : public ICommand
{
public:
	explicit TurnOnEngineCommand(Car& car)
		: m_car(car)
	{
	}

	void Execute() override
	{
		m_car.TurnOnEngine();
	}

private:
	Car& m_car;
};