#pragma once
#include "Environment.hpp"
#include "IExpression.hpp"

#include <cctype>
#include <limits>
#include <ranges>
#include <stdexcept>
#include <unordered_map>

class Calculator
{
public:
	struct Function
	{
		std::string left;
		std::string right;
		Operation operation = Operation::None;
	};

	void DeclareVariable(const std::string& id)
	{
		if (!IsValidVariable(id))
		{
			throw std::invalid_argument("Invalid variable name: " + id);
		}

		if (m_env.Exists(id))
		{
			throw std::invalid_argument("Variable already exists: " + id);
		}

		m_env.SetVariable(id, std::numeric_limits<double>::quiet_NaN());
	}

	void AssignVariable(const std::string& id, double value)
	{
		if (m_env.IsFunction(id))
		{
			throw std::invalid_argument("Cannot assign to function: " + id);
		}

		if (!m_env.Exists(id))
		{
			DeclareVariable(id);
		}

		m_env.SetVariable(id, value);
	}

	void AssignVariable(const std::string& id1, const std::string& id2)
	{
		AssignVariable(id1, GetValue(id2));
	}

	void DefineFunction(const std::string& id, const std::string& operand)
	{
		if (!IsValidVariable(id))
		{
			throw std::invalid_argument("Invalid function name: " + id);
		}

		if (m_env.Exists(id))
		{
			throw std::invalid_argument("Variable already exists: " + id);
		}

		if (!IsOperandValid(operand))
		{
			throw std::invalid_argument("Invalid operand encountered");
		}

		if (IsNumber(operand))
		{
			m_env.SetFunction(id, std::make_unique<NumberExpression>(std::stod(operand)));
		}
		else
		{
			m_env.SetFunction(id, std::make_unique<VariableExpression>(operand));
		}
	}

	void DefineFunction(const std::string& id, const Function& function)
	{
		if (!IsValidVariable(id))
		{
			throw std::invalid_argument("Invalid function name: " + id);
		}

		if (m_env.Exists(id))
		{
			throw std::invalid_argument("Variable already exists: " + id);
		}

		if (!IsOperandValid(function.left) || !IsOperandValid(function.right))
		{
			throw std::invalid_argument("Invalid operand encountered");
		}

		Operation operation = MapOperation(function.operation);

		m_env.SetFunction(
			id,
			std::make_unique<BinaryExpression>(
				operation,
				CreateExpression(function.left),
				CreateExpression(function.right)));
	}

	double GetValue(const std::string& id)
	{
		return m_env.GetValue(id);
	}

	std::unordered_map<std::string, double> GetAllVariables() const
	{
		return m_env.GetAllVariables();
	}

	std::unordered_map<std::string, double> GetAllFunctions()
	{
		std::unordered_map<std::string, double> result;

		for (const auto& name : m_env.GetAllFunctions() | std::views::keys)
		{
			result[name] = m_env.GetValue(name);
		}

		return result;
	}

private:
	Environment m_env;

	static Operation MapOperation(Operation operation)
	{
		switch (operation)
		{
		case Operation::Add:
			return Operation::Add;
		case Operation::Sub:
			return Operation::Sub;
		case Operation::Mul:
			return Operation::Mul;
		case Operation::Div:
			return Operation::Div;
		default:
			throw std::logic_error("Undefined operation");
		}
	}

	static bool IsValidVariable(const std::string& id)
	{
		if (id.empty() || std::isdigit(static_cast<unsigned char>(id[0])))
		{
			return false;
		}

		for (const char c : id)
		{
			if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_')
			{
				return false;
			}
		}

		return true;
	}

	bool IsOperandValid(const std::string& operand) const
	{
		if (m_env.Exists(operand))
		{
			return true;
		}

		return IsNumber(operand);
	}

	static bool IsNumber(const std::string& string)
	{
		try
		{
			size_t pos;
			std::stod(string, &pos);
			return pos == string.size();
		}
		catch (...)
		{
			return false;
		}
	}

	static std::unique_ptr<IExpression> CreateExpression(const std::string& operand)
	{
		if (IsNumber(operand))
		{
			return std::make_unique<NumberExpression>(std::stod(operand));
		}

		return std::make_unique<VariableExpression>(operand);
	}
};