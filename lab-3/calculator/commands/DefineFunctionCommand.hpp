#pragma once

#include "../Calculator.hpp"
#include "../ParseArguments.hpp"
#include "ICommand.hpp"

#include <stdexcept>

class DefineFunctionCommand : public ICommand
{
public:
	explicit DefineFunctionCommand(Calculator& calc)
		: m_calc(calc)
	{
	}

	void ParseArgs(const std::vector<std::string>& args) override
	{
		const auto arguments = ParseArguments(args);

		if (arguments.size() < 2)
		{
			throw std::invalid_argument("Usage: fn <id>=<id2> OR fn <id>=<id1><op><id2>");
		}

		m_fnId = arguments[0];
		if (arguments[1] != "=")
		{
			throw std::invalid_argument("Expected '=' after variable name");
		}

		m_leftId = arguments[2];

		if (arguments.size() == 5)
		{
			m_op = ParseOperation(arguments[3]);
			m_rightId = arguments[4];
			m_isBinary = true;
		}
	}

	void Execute() override
	{
		if (m_isBinary)
		{
			const Calculator::Function function{ m_leftId, m_rightId, m_op };
			m_calc.DefineFunction(m_fnId, function);
		}
		else
		{
			m_calc.DefineFunction(m_fnId, m_leftId);
		}
	}

private:
	Calculator& m_calc;
	std::string m_fnId, m_leftId, m_rightId;
	Calculator::Operation m_op = Calculator::Operation::None;
	bool m_isBinary = false;

	static Calculator::Operation ParseOperation(const std::string& operation)
	{
		static const std::map<std::string, Calculator::Operation> operations = {
			{ "+", Calculator::Operation::Add },
			{ "-", Calculator::Operation::Sub },
			{ "*", Calculator::Operation::Mul },
			{ "/", Calculator::Operation::Div }
		};

		if (const auto it = operations.find(operation); it != operations.end())
		{
			return it->second;
		}

		throw std::invalid_argument("Invalid operation");
	}
};