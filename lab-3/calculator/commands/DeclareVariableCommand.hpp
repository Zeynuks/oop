#pragma once

#include "../ParseArguments.hpp"
#include "../Calculator.hpp"
#include "ICommand.hpp"

#include <stdexcept>

class DeclareVariableCommand : public ICommand
{
public:
	explicit DeclareVariableCommand(Calculator& calc)
		: m_calc(calc)
	{
	}

	void ParseArgs(const std::vector<std::string>& args) override
	{
		const auto arguments = ParseArguments(args);

		if (arguments.empty())
		{
			throw std::invalid_argument("Usage: var <identifier>");
		}

		if (arguments.size() > 1)
		{
			throw std::invalid_argument("Too many arguments");
		}

		m_id = arguments[0];
	}

	void Execute() override
	{
		m_calc.DeclareVariable(m_id);
	}

private:
	Calculator& m_calc;
	std::string m_id;
};