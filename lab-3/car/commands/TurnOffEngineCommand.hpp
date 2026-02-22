#pragma once

#include "../Car.hpp"
#include "ICommand.hpp"

class TurnOffEngineCommand : public ICommand
{
public:
	explicit TurnOffEngineCommand(Car& car)
		: m_car(car)
	{
	}

	void Execute() override
	{
		m_car.TurnOffEngine();
	}

private:
	Car& m_car;
};