#pragma once

#include "IPerson.hpp"

#include <memory>

// базовое транспортное средство
class IBasicVehicle
{
public:
	virtual ~IBasicVehicle() = default;

	// сигнализирует о том, пусто ли транспортное средство
	virtual bool IsEmpty()const = 0;

	// сигнализирует о том заполнено ли транспортное средство полностью
	virtual bool IsFull()const = 0;

	// возвращает общее количество мест
	virtual size_t GetPlaceCount()const = 0;

	// возвращает количество пассажиров на борту
	virtual size_t GetPassengerCount()const = 0;

	// высаживает всех пассажиров
	virtual void RemoveAllPassengers() = 0;
};

// транспортное средство предназначенное для провозки заданного типа пассажиров
template <typename Passenger>
class IVehicle : public IBasicVehicle
{
public:
	// добавить пассажира на борт
	// т.к. пассажир может быть полиморфным типом, принимаем его по умному указателю
	// Если нет места, выбрасывается исключение std::logic_error
	virtual void AddPassenger(std::shared_ptr<Passenger> pPassenger) = 0;

	// Получить ссылку на пассажира с заданным индексом
	// выбрасывает исключение std::out_of_range в случае недопустимого индекса
	virtual Passenger const& GetPassenger(size_t index)const = 0;

	// убрать пассажира с заданным индексом
	// выбрасывает исключение std::out_of_range в случае недопустимого индекса
	virtual void RemovePassenger(size_t index) = 0;
};

enum class MakeOfTheCar
{
	Bmw,
	Mitsubishi,
	Ford,
	Mercedes,
	Toyota,
	Kia,
	Ferrari,
	Porsche,
	Lexus,
	Nissan,
	Infiniti
};

// автомобиль, перевозящий заданный тип пассажиров
template <typename Passenger>
class ICar : public IVehicle<Passenger>
{
public:
	virtual MakeOfTheCar GetMakeOfTheCar()const = 0;
};

// марка автобуса
class IBus : public IVehicle<IPerson>
{
};

class IPoliceCar : public ICar<IPoliceMan>
{
};

class IRacingCar : public ICar<IRacer>
{
};

class ITaxi : public ICar<IPerson>
{
};