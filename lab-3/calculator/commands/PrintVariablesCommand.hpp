#pragma once

#include "../ParseArguments.hpp"
#include "../core/Calculator.hpp"
#include "ICommand.hpp"

#include <iostream>

class PrintVariablesCommand : public ICommand
{
public:
	PrintVariablesCommand(Calculator& calc, std::ostream& out)
		: m_calc(calc)
		, m_out(out)
	{
	}

	void ParseArgs(const std::vector<std::string>& args) override
	{

		if (const auto arguments = ParseArguments(args); !arguments.empty())
		{
			throw std::invalid_argument("Too many arguments");
		}
	}

	void Execute() override
	{
		const auto variables = m_calc.GetAllVariables();

		if (variables.empty())
		{
			throw std::logic_error("No variables found");
		}

		for (const auto& [name, value] : variables)
		{
			m_out << "Function: " << name << ":" << value<< std::endl;
		}
	}

private:
	Calculator& m_calc;
	std::ostream& m_out;
};