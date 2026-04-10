#pragma once

#include "BasePerson.hpp"

class Person : public BasePerson<IPerson>
{
public:
	Person(const std::string& name)
		: BasePerson(name)
	{
	}
};

class PoliceMan : public BasePerson<IPoliceMan>
{
public:
	PoliceMan(const std::string& name, const std::string& department)
		: BasePerson(name)
		, m_department(department)
	{
	}

	std::string GetDepartmentName() const override
	{
		return m_department;
	}

private:
	std::string m_department;
};

class Racer : public BasePerson<IRacer>
{
public:
	Racer(const std::string& name, const size_t awardsCount)
		: BasePerson(name)
		, m_awardsCount(awardsCount)
	{
	}

	size_t GetAwardsCount() const override
	{
		return m_awardsCount;
	}

private:
	size_t m_awardsCount;
};