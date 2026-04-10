#pragma once

#include "IVehicle.hpp"

template <typename TBase, typename TPassenger>
	requires std::is_base_of_v<ICar<TPassenger>, TBase>
class BaseCar : public TBase
{
public:
	BaseCar(size_t placeCount, const MakeOfTheCar makeOfCar)
		: TBase(placeCount)
		, m_makeOfCar(makeOfCar)
	{
	}

	MakeOfTheCar GetMakeOfTheCar() const override
	{
		return m_makeOfCar;
	}

private:
	MakeOfTheCar m_makeOfCar;
};