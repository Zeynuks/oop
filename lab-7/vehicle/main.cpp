#include "core/IVehicle.hpp"
#include "core/Person.hpp"
#include "core/Vehicle.hpp"

#include <iostream>
#include <memory>

template <typename T>
	requires std::is_base_of_v<IPerson, T>
void PrintPassengersInfo(const IVehicle<T>& vehicle)
{
	for (size_t i = 0; i < vehicle.GetPassengerCount(); ++i)
	{
		auto& p = vehicle.GetPassenger(i);
		std::cout << p.GetName() << std::endl;
	}

	std::cout << std::endl;
}

int main()
{
	const auto john = std::make_shared<PoliceMan>("John Smith", "North-west department");
	PoliceCar johnCar(5, MakeOfTheCar::Ford);
	johnCar.AddPassenger(john);

	const auto jim = std::make_shared<PoliceMan>("Jim Clark", "South-east department");

	johnCar.AddPassenger(jim);

	for (size_t i = 0; i < johnCar.GetPassengerCount(); ++i)
	{
		auto& p = johnCar.GetPassenger(i);

		std::cout << p.GetName();
		std::cout << " from " << p.GetDepartmentName() << std::endl;
	}
	std::cout << std::endl;

	johnCar.RemovePassenger(1);

	const auto gandhi = std::make_shared<Person>("Raja Gandhi");
	const auto michael = std::make_shared<Racer>("Michael Schumacher", 999);
	Taxi taxi(2, MakeOfTheCar::Toyota);

	taxi.AddPassenger(gandhi);
	taxi.AddPassenger(michael);
	std::cout << "Taxi before John" << std::endl;
	PrintPassengersInfo(taxi);

	taxi.RemovePassenger(0);
	taxi.AddPassenger(john);

	std::cout << "Taxi after John" << std::endl;
	PrintPassengersInfo(taxi);

	try
	{
		taxi.AddPassenger(gandhi);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}