#pragma once

#include "../ParseArguments.hpp"
#include "../Calculator.hpp"
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

		m_valueOrId2 = arguments[2];
	}

	void Execute() override
	{
		const double value = std::stod(m_valueOrId2);
		m_calc.AssignVariable(m_id1, value);
	}

private:
	Calculator& m_calc;
	std::string m_id1;
	std::string m_valueOrId2;
};