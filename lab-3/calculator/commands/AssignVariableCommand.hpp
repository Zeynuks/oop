#pragma once

#include "../core/Calculator.hpp"
#include "../ParseArguments.hpp"
#include "ICommand.hpp"

#include <stdexcept>
#include <string>

class AssignVariableCommand : public ICommand
{
public:
	explicit AssignVariableCommand(Calculator& calc)
		: m_calc(calc)
	{
	}

	void ParseArgs(const std::vector<std::string>& args) override
	{
		const auto arguments = ParseArguments(args);

		if (arguments.empty())
		{
			throw std::invalid_argument("Usage: let <id> <value|id2>");
		}

		m_id1 = arguments[0];
		if (arguments[1] != "=")
		{
			throw std::invalid_argument("Expected '=' after variable name");
		}

		if (arguments.size() > 3)
		{
			throw std::invalid_argument("Too many arguments");
		}

		m_valueOrId2 = arguments[2];
	}

	void Execute() override
	{
		try
		{
			size_t pos;
			const double value = std::stod(m_valueOrId2, &pos);

			if (pos != m_valueOrId2.size())
			{
				throw std::invalid_argument("Extra characters after number");
			}

			m_calc.AssignVariable(m_id1, value);
		}
		catch (const std::invalid_argument&)
		{
			m_calc.AssignVariable(m_id1, m_valueOrId2);
		}
	}

private:
	Calculator& m_calc;
	std::string m_id1;
	std::string m_valueOrId2;
};