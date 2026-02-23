#pragma once

#include "../ParseArguments.hpp"
#include "../core/Calculator.hpp"
#include "ICommand.hpp"

#include <iomanip>
#include <iostream>

class PrintCommand : public ICommand
{
public:
	PrintCommand(Calculator& calc, std::ostream& out)
		: m_calc(calc)
		, m_out(out)
	{
	}

	void ParseArgs(const std::vector<std::string>& args) override
	{
		const auto arguments = ParseArguments(args);

		if (arguments.empty())
		{
			throw std::invalid_argument("Usage: print <id>");
		}

		if (arguments.size() > 1)
		{
			throw std::invalid_argument("Too many arguments");
		}

		m_id = arguments[0];
	}

	void Execute() override
	{
		const double val = m_calc.GetValue(m_id);
		m_out << std::fixed << std::setprecision(2) << val << std::endl;
	}

private:
	Calculator& m_calc;
	std::ostream& m_out;
	std::string m_id;
};