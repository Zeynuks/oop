#pragma once

#include <string>

class IValueProvider
{
public:
	virtual ~IValueProvider() = default;
	virtual double GetValue(const std::string& id) = 0;
};