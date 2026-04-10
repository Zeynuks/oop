#pragma once

#include "IVehicle.hpp"

#include <stdexcept>
#include <vector>

template <typename TBase, typename TPassenger>
	requires std::is_base_of_v<IVehicle<TPassenger>, TBase>
class BaseVehicle : public TBase
{
public:
	explicit BaseVehicle(size_t placeCount)
		: m_placeCount(placeCount)
	{
		m_passengers.reserve(placeCount);
	}

	bool IsEmpty() const override
	{
		return m_passengers.empty();
	}

	bool IsFull() const override
	{
		return m_passengers.size() >= m_placeCount;
	}

	size_t GetPlaceCount() const override
	{
		return m_placeCount;
	}

	size_t GetPassengerCount() const override
	{
		return m_passengers.size();
	}

	void RemoveAllPassengers() override
	{
		m_passengers.clear();
	}

	void AddPassenger(std::shared_ptr<TPassenger> passenger) override
	{
		if (IsFull())
		{
			throw std::logic_error("Vehicle is full");
		}

		m_passengers.push_back(passenger);
	}

	const TPassenger& GetPassenger(size_t index) const override
	{
		if (index >= m_passengers.size())
		{
			throw std::out_of_range("Index is out of range");
		}

		return *m_passengers[index];
	}

	void RemovePassenger(size_t index) override
	{
		if (index >= m_passengers.size())
		{
			throw std::out_of_range("Index is out of range");
		}

		m_passengers.erase(m_passengers.begin() + index);
	}

private:
	std::vector<std::shared_ptr<TPassenger>> m_passengers;
	size_t m_placeCount;
};