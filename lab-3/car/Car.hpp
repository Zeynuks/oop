#pragma once

class Car
{
public:
	enum class Gear
	{
		Reverse = -1,
		Neutral = 0,
		First = 1,
		Second = 2,
		Third = 3,
		Fourth = 4,
		Fifth = 5
	};

	enum class Direction
	{
		Still,
		Forward,
		Backward
	};

	Car();

	[[nodiscard]] bool IsTurnedOn() const;
	[[nodiscard]] Direction GetDirection() const;
	[[nodiscard]] int GetSpeed() const;
	[[nodiscard]] Gear GetGear() const;

	void TurnOnEngine();
	void TurnOffEngine();
	void SetGear(Gear gear);
	void SetSpeed(int speed);

private:
	Direction m_direction = Direction::Still;
	Gear m_gear = Gear::Neutral;
	bool m_engineOn = false;
	int m_speed = 0;

	void ChangeDirection(int speed);
};