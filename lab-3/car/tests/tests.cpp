#include <gtest/gtest.h>
#include "../Car.hpp"
#include <stdexcept>

class CarTest : public ::testing::Test {
protected:
    Car car;
};

TEST_F(CarTest, EngineOnOff)
{
    EXPECT_FALSE(car.IsTurnedOn());
    car.TurnOnEngine();
    EXPECT_TRUE(car.IsTurnedOn());
    car.SetGear(Car::Gear::Neutral);
    car.SetSpeed(0);
    EXPECT_NO_THROW(car.TurnOffEngine());
    EXPECT_FALSE(car.IsTurnedOn());
}

TEST_F(CarTest, TurnOffEngineFailsIfMovingOrNotNeutral)
{
    car.TurnOnEngine();
    car.SetGear(Car::Gear::First);
    car.SetSpeed(10);
    EXPECT_THROW(car.TurnOffEngine(), std::logic_error);
    car.SetSpeed(0);
    car.SetGear(Car::Gear::First);
    EXPECT_THROW(car.TurnOffEngine(), std::logic_error);
}

TEST_F(CarTest, SetGearValidAndInvalid)
{
    car.TurnOnEngine();
    EXPECT_NO_THROW(car.SetGear(Car::Gear::Reverse));
    car.SetSpeed(0);
    EXPECT_NO_THROW(car.SetGear(Car::Gear::First));
    EXPECT_NO_THROW(car.SetGear(Car::Gear::Neutral));
    EXPECT_THROW(car.SetGear(static_cast<Car::Gear>(-2)), std::logic_error);
    EXPECT_THROW(car.SetGear(static_cast<Car::Gear>(6)), std::logic_error);
    car.SetGear(Car::Gear::First);
    car.SetSpeed(10);
    EXPECT_THROW(car.SetGear(Car::Gear::Reverse), std::logic_error);
    car.SetGear(Car::Gear::Neutral);
    car.SetSpeed(0);
    car.TurnOffEngine();
    EXPECT_NO_THROW(car.SetGear(Car::Gear::Neutral));
    EXPECT_THROW(car.SetGear(Car::Gear::First), std::logic_error);
}

TEST_F(CarTest, SetSpeedValidAndInvalid)
{
    car.TurnOnEngine();
    car.SetGear(Car::Gear::First);
    car.SetSpeed(0);
    EXPECT_EQ(car.GetSpeed(), 0);
    car.SetSpeed(30);
    EXPECT_EQ(car.GetSpeed(), 30);
    EXPECT_THROW(car.SetSpeed(31), std::logic_error);
    EXPECT_THROW(car.SetSpeed(-1), std::logic_error);
    car.SetGear(Car::Gear::Neutral);
    car.SetSpeed(20);
    EXPECT_THROW(car.SetSpeed(25), std::logic_error);
    EXPECT_NO_THROW(car.SetSpeed(15));
}

TEST_F(CarTest, DirectionChanges)
{
	car.TurnOnEngine();
	car.SetSpeed(0);
	car.SetGear(Car::Gear::Reverse);
	car.SetSpeed(10);
	EXPECT_EQ(car.GetDirection(), Car::Direction::Backward);
	car.SetSpeed(0);
	EXPECT_EQ(car.GetDirection(), Car::Direction::Still);
	car.SetSpeed(0);
	car.SetGear(Car::Gear::First);
	car.SetSpeed(10);
	EXPECT_EQ(car.GetDirection(), Car::Direction::Forward);
	car.SetSpeed(10);
	car.SetGear(Car::Gear::Neutral);
	EXPECT_EQ(car.GetDirection(), Car::Direction::Forward);
	car.SetSpeed(0);
	EXPECT_EQ(car.GetDirection(), Car::Direction::Still);
}

TEST_F(CarTest, FullSpeedAndGearRange)
{
	car.TurnOnEngine();
	car.SetGear(Car::Gear::First);
	car.SetSpeed(30);
	EXPECT_NO_THROW(car.SetGear(Car::Gear::Second));
	car.SetSpeed(50);
	EXPECT_NO_THROW(car.SetGear(Car::Gear::Third));
	car.SetSpeed(60);
	EXPECT_NO_THROW(car.SetGear(Car::Gear::Fourth));
	car.SetSpeed(90);
	EXPECT_NO_THROW(car.SetGear(Car::Gear::Fifth));
	car.SetSpeed(150);
	EXPECT_THROW(car.SetSpeed(151), std::logic_error);
}