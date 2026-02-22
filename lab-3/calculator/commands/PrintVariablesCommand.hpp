#pragma once

#include "../ParseArguments.hpp"
#include "../Calculator.hpp"
#include "ICommand.hpp"

#include <iomanip>
#include <iostream>

class PrintVariablesCommand : public ICommand
{
public:
	PrintVariablesCommand(Calculator& calc, std::ostream& out)
		: m_calc(calc)
		, m_out(out)
	{
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
			m_out << "Function: " << name << ":" << value;
		}

		m_out << std::endl;
	}

private:
	Calculator& m_calc;
	std::ostream& m_out;
};