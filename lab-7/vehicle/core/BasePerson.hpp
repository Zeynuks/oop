#pragma once

#include "IPerson.hpp"

template <typename TBase>
	requires std::is_base_of_v<IPerson, TBase>
class BasePerson : public TBase
{
public:
	explicit BasePerson(const std::string& name)
		: m_name(name)
	{
	}

	std::string GetName() const override
	{
		return m_name;
	}

private:
	std::string m_name;
};