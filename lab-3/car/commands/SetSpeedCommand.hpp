#pragma once

#include "../Car.hpp"
#include "ICommand.hpp"

#include <stdexcept>
#include <string>

class SetSpeedCommand : public ICommand
{
public:
	explicit SetSpeedCommand(Car& car)
		: m_car(car)
	{
	}

	void ParseArgs(const std::vector<std::string>& args) override
	{
		if (args.empty())
			throw std::runtime_error("SetSpeed requires 1 argument");

		try
		{
			m_speed = std::stoi(args[0]);
		}
		catch (const std::exception&)
		{
			throw std::runtime_error("Invalid argument");
		}
	}

	void Execute() override
	{
		m_car.SetSpeed(m_speed);
	}

private:
	Car& m_car;
	int m_speed = 0;
};