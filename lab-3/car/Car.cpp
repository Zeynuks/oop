#include "Car.hpp"

#include <stdexcept>
#include <unordered_map>

using SpeedLimits = std::unordered_map<Car::Gear, std::pair<int, int>>;

const SpeedLimits GEAR_SPEED_RANGES = {
	{ Car::Gear::Reverse, { 0, 20 } },
	{ Car::Gear::Neutral, { 0, 150 } },
	{ Car::Gear::First, { 0, 30 } },
	{ Car::Gear::Second, { 20, 50 } },
	{ Car::Gear::Third, { 30, 60 } },
	{ Car::Gear::Fourth, { 40, 90 } },
	{ Car::Gear::Fifth, { 50, 150 } },
};

Car::Car()
{
}

bool Car::IsTurnedOn() const
{
	return m_engineOn;
}

Car::Direction Car::GetDirection() const
{
	return m_direction;
}

int Car::GetSpeed() const
{
	return m_speed;
}

Car::Gear Car::GetGear() const
{
	return m_gear;
}

void Car::TurnOnEngine()
{
	m_engineOn = true;
}

void Car::TurnOffEngine()
{
	if (m_gear != Gear::Neutral || m_speed != 0)
	{
		throw std::logic_error("Сar must be stopped and in neutral gear");
	}

	m_engineOn = false;
}

void Car::SetGear(const Gear gear)
{
	if (gear < Gear::Reverse || gear > Gear::Fifth)
	{
		throw std::logic_error("Invalid gear value");
	}

	if (gear == Gear::Reverse && m_speed != 0)
	{
		throw std::logic_error("Cannot switch to reverse while moving");
	}

	if (!m_engineOn && gear != Gear::Neutral)
	{
		throw std::logic_error("Cannot set gear when engine is off");
	}

	if (auto& [minSpeed, maxSpeed] = GEAR_SPEED_RANGES.at(gear); m_speed < minSpeed || m_speed > maxSpeed)
	{
		throw std::logic_error("Current speed is not allowed for this gear");
	}

	m_gear = gear;
}

void Car::SetSpeed(const int speed)
{
	if (speed < 0)
	{
		throw std::logic_error("Speed cannot be negative");
	}

	if (!m_engineOn)
	{
		throw std::logic_error("Cannot change speed when engine is off");
	}

	if (m_gear == Gear::Neutral && speed > m_speed)
	{
		throw std::logic_error("Cannot accelerate on neutral gear");
	}

	if (auto& [minSpeed, maxSpeed] = GEAR_SPEED_RANGES.at(m_gear); speed < minSpeed || speed > maxSpeed)
	{
		throw std::logic_error("Speed is out of range for current gear");
	}

	ChangeDirection(speed);
	m_speed = speed;
}

void Car::ChangeDirection(const int speed)
{

	if (speed == 0)
	{
		m_direction = Direction::Still;
	}
	else if (m_gear == Gear::Reverse || (m_direction == Direction::Backward && m_gear == Gear::Neutral))
	{
		m_direction = Direction::Backward;
	}
	else
	{
		m_direction = Direction::Forward;
	}
}