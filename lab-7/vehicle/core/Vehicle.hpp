#pragma once

#include "BaseCar.hpp"
#include "BaseVehicle.hpp"

class Taxi : public BaseCar<BaseVehicle<ITaxi, IPerson>, IPerson>
{
public:
	Taxi(const size_t placeCount, const MakeOfTheCar makeOfCar)
		: BaseCar(placeCount, makeOfCar)
	{
	}
};

class PoliceCar : public BaseCar<BaseVehicle<IPoliceCar, IPoliceMan>, IPoliceMan>
{
public:
	PoliceCar(const size_t placeCount, const MakeOfTheCar makeOfCar)
		: BaseCar(placeCount, makeOfCar)
	{
	}
};

class RacingCar : public BaseCar<BaseVehicle<IRacingCar, IRacer>, IRacer>
{
public:
	RacingCar(const size_t placeCount, const MakeOfTheCar makeOfCar)
		: BaseCar(placeCount, makeOfCar)
	{
	}
};

class Bus : public BaseVehicle<IBus, IPerson>
{
public:
	Bus(const size_t placeCount)
		: BaseVehicle(placeCount)
	{
	}
};