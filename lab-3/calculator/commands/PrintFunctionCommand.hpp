#pragma once

#include "../ParseArguments.hpp"
#include "../Calculator.hpp"
#include "ICommand.hpp"

#include <iomanip>
#include <iostream>

class PrintFunctionCommand : public ICommand
{
public:
	PrintFunctionCommand(Calculator& calc, std::ostream& out)
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
		const auto functions = m_calc.GetAllFunctions();

		if (functions.empty())
		{
			throw std::logic_error("No functions found");
		}

		for (const auto& [name, value] : functions)
		{
			m_out << "Function: " << name << ":" << value;
		}

		m_out << std::endl;
	}

private:
	Calculator& m_calc;
	std::ostream& m_out;
};