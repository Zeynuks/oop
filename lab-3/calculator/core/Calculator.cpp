#include "Calculator.hpp"
#include <limits>
#include <ranges>
#include <stdexcept>

void Calculator::DeclareVariable(const std::string& id)
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

void Calculator::AssignVariable(const std::string& id, const double value)
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

void Calculator::AssignVariable(const std::string& id1, const std::string& id2)
{
	AssignVariable(id1, GetValue(id2));
}

void Calculator::DefineFunction(const std::string& id, const std::string& operand)
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

	m_env.SetFunction(id, std::make_shared<VariableExpression>(operand));
}

void Calculator::DefineFunction(const std::string& id, const Function& function)
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
	m_env.SetFunction(id, std::make_shared<BinaryExpression>(operation, function.left, function.right));
}

double Calculator::GetValue(const std::string& id)
{
	return m_env.GetValue(id);
}

const std::unordered_map<std::string, double>& Calculator::GetAllVariables() const
{
	return m_env.GetAllVariables();
}

std::unordered_map<std::string, double> Calculator::GetAllFunctions()
{
	std::unordered_map<std::string, double> res;
	for (const auto& name : m_env.GetAllFunctions() | std::views::keys)
	{
		res[name] = m_env.GetValue(name);
	}

	return res;
}

Operation Calculator::MapOperation(const Operation operation)
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

bool Calculator::IsValidVariable(const std::string& id)
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

bool Calculator::IsOperandValid(const std::string& operation) const
{
	if (m_env.Exists(operation))
	{
		return true;
	}
	try
	{
		size_t pos;
		std::stod(operation, &pos);

		return pos == operation.size();
	}
	catch (...)
	{
		return false;
	}
}