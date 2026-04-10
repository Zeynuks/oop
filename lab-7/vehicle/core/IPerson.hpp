#pragma once

#include <string>

// человек
class IPerson
{
public:
	// имя
	virtual std::string GetName()const = 0;
	virtual ~IPerson() = default;
};

// полицейский
class IPoliceMan : public IPerson
{
public:
	// название полицейского департамента
	virtual std::string GetDepartmentName()const = 0;
};

// гонщик
class IRacer : public IPerson
{
public:
	// количество наград
	virtual size_t GetAwardsCount()const = 0;
};