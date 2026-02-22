#pragma once

#include "../Car.hpp"
#include "ICommand.hpp"

#include <iostream>

class CarInfoCommand : public ICommand
{
public:
	explicit CarInfoCommand(Car& car)
		: m_car(car)
	{
	}

	void Execute() override
	{
		const std::string isEngineOn = m_car.IsTurnedOn() ? "on" : "off";

		std::cout << "Engine: " << isEngineOn << std::endl;
		std::cout << "Direction: " << DirectionToString(m_car.GetDirection()) << std::endl;
		std::cout << "Speed: " << m_car.GetSpeed() << std::endl;
		std::cout << "Gear: " << GearToString(m_car.GetGear()) << std::endl;
	}

private:
	Car& m_car;

	static std::string DirectionToString(const Car::Direction direction)
	{
		switch (direction)
		{
		case Car::Direction::Still:
			return "Still";
		case Car::Direction::Forward:
			return "Forward";
		case Car::Direction::Backward:
			return "Backward";
		default:
			throw std::invalid_argument("Unknown direction");
		}
	}

	static std::string GearToString(const Car::Gear gear)
	{
		switch (gear)
		{
		case Car::Gear::Reverse:
			return "Reverse";
		case Car::Gear::Neutral:
			return "Neutral";
		case Car::Gear::First:
			return "First";
		case Car::Gear::Second:
			return "Second";
		case Car::Gear::Third:
			return "Third";
		case Car::Gear::Fourth:
			return "Fourth";
		case Car::Gear::Fifth:
			return "Fifth";
		default:
			throw std::invalid_argument("Unknown gear");
		}
	}
};