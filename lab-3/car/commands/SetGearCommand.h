#pragma once

#include "../Car.hpp"
#include "ICommand.hpp"

#include <stdexcept>

class SetGearCommand : public ICommand
{
public:
	explicit SetGearCommand(Car& car)
		: m_car(car)
	{
	}

	void ParseArgs(const std::vector<std::string>& args) override
	{
		if (args.empty())
		{
			throw std::runtime_error("SetGear requires 1 argument");
		}

		int gearInt;
		try
		{
			gearInt = std::stoi(args[0]);
		}
		catch (const std::exception&)
		{
			throw std::runtime_error("Invalid argument");
		}

		m_gear = ParseGear(gearInt);
	}

	void Execute() override
	{
		m_car.SetGear(m_gear);
	}

private:
	Car& m_car;
	Car::Gear m_gear = Car::Gear::Neutral;

	static Car::Gear ParseGear(const int gear)
	{
		switch (gear)
		{
		case -1:
			return Car::Gear::Reverse;
		case 0:
			return Car::Gear::Neutral;
		case 1:
			return Car::Gear::First;
		case 2:
			return Car::Gear::Second;
		case 3:
			return Car::Gear::Third;
		case 4:
			return Car::Gear::Fourth;
		case 5:
			return Car::Gear::Fifth;
		default:
			throw std::logic_error("Invalid command argument");
		}
	}
};