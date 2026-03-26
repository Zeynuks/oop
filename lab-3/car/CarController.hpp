#pragma once

#include "Car.hpp"

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace CarController
{
namespace Detail
{
inline std::string DirectionToString(const Car::Direction direction)
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
		return "Unknown";
	}
}

inline std::string GearToString(const Car::Gear gear)
{
	static const std::map<Car::Gear, std::string> names = {
		{ Car::Gear::Reverse, "Reverse" }, { Car::Gear::Neutral, "Neutral" },
		{ Car::Gear::First, "First" }, { Car::Gear::Second, "Second" },
		{ Car::Gear::Third, "Third" }, { Car::Gear::Fourth, "Fourth" },
		{ Car::Gear::Fifth, "Fifth" }
	};
	return names.contains(gear) ? names.at(gear) : "Unknown";
}
} // namespace Detail

inline void ShowInfo(const Car& car)
{
	std::cout << "Engine: " << (car.IsTurnedOn() ? "on" : "off") << std::endl
			  << "Direction: " << Detail::DirectionToString(car.GetDirection()) << std::endl
			  << "Speed: " << car.GetSpeed() << std::endl
			  << "Gear: " << Detail::GearToString(car.GetGear()) << std::endl;
}

inline void TurnOnEngine(Car& car)
{
	car.TurnOnEngine();
	std::cout << "Engine is turned on" << std::endl;
}

inline void TurnOffEngine(Car& car)
{
	car.TurnOffEngine();
	std::cout << "Engine is turned off" << std::endl;
}

inline void SetGear(Car& car, const std::vector<std::string>& args)
{
	if (args.empty())
	{
		throw std::invalid_argument("SetGear requires 1 argument (gear number)");
	}

	const int gearValue = std::stoi(args[0]);
	if (gearValue < -1 || gearValue > 5)
	{
		throw std::invalid_argument("Invalid gear level: use -1 to 5");
	}

	car.SetGear(static_cast<Car::Gear>(gearValue));
	std::cout << "Gear shifted to " << gearValue << std::endl;
}

inline void SetSpeed(Car& car, const std::vector<std::string>& args)
{
	if (args.empty())
	{
		throw std::invalid_argument("SetSpeed requires 1 argument");
	}

	const int speed = std::stoi(args[0]);
	car.SetSpeed(speed);
	std::cout << "Speed set to " << speed << std::endl;
}
} // namespace CarController