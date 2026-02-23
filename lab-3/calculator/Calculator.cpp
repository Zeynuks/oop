#include "Calculator.hpp"
#include <ranges>
#include <stdexcept>

constexpr double NAN_VALUE = std::numeric_limits<double>::quiet_NaN();

bool Calculator::DeclareVariable(const std::string& id)
{
	if (!IsValidVariable(id))
	{
		throw std::invalid_argument("Invalid variable name: '" + id + "'");
	}

	if (Exists(id))
	{
		throw std::invalid_argument("Variable '" + id + "' already exists");
	}

	m_variables[id] = NAN_VALUE;

	return true;
}

bool Calculator::AssignVariable(const std::string& id, const double value)
{
	if (m_functions.contains(id))
	{
		throw std::invalid_argument("Cannot assign value to function '" + id + "'");
	}
	if (!m_variables.contains(id))
	{
		throw std::invalid_argument("Variable '" + id + "' does not exist");
	}

	m_variables[id] = value;
	ClearCache();

	return true;
}

bool Calculator::AssignVariableFromId(const std::string& id1, const std::string& id2)
{
	if (m_functions.contains(id1))
	{
		throw std::invalid_argument("Cannot assign value to function '" + id1 + "'");
	}
	if (!m_variables.contains(id1))
	{
		throw std::invalid_argument("Variable '" + id1 + "' does not exist");
	}

	const double val = GetValue(id2);
	m_variables[id1] = val;
	ClearCache();

	return true;
}

bool Calculator::DefineFunction(const std::string& id, const std::string& operand)
{
	if (!IsValidVariable(id))
	{
		throw std::invalid_argument("Invalid variable name: '" + id + "'");
	}

	if (Exists(id))
	{
		throw std::invalid_argument("Variable '" + id + "' already exists");
	}

	if (!IsOperandValid(operand))
	{
		throw std::invalid_argument("Operand '" + operand + "' does not exist");
	}

	m_functions[id] = { operand, "", Operation::None, false };

	return true;
}

bool Calculator::DefineFunction(const std::string& id, const Function& function)
{
	if (!IsValidVariable(id))
	{
		throw std::invalid_argument("Invalid function name: '" + id + "'");
	}

	if (Exists(id))
	{
		throw std::invalid_argument("Variable '" + id + "' already exists");
	}

	if (!IsOperandValid(function.left))
	{
		throw std::invalid_argument("Left operand '" + function.left + "' does not exist");
	}

	if (function.isBinary && !IsOperandValid(function.right))
	{
		throw std::invalid_argument("Right operand '" + function.right + "' does not exist");
	}

	m_functions[id] = { function.left, function.right, function.operation, true };

	return true;
}

double Calculator::GetValue(const std::string& id)
{
	if (m_variables.contains(id))
	{
		return m_variables.at(id);
	}

	if (m_functions.contains(id))
	{
		return GetCalculatedFunctionValue(id);
	}

	throw std::invalid_argument("Variable '" + id + "' not found");
}

const std::map<std::string, double>& Calculator::GetAllVariables() const
{
	return m_variables;
}

std::map<std::string, double> Calculator::GetAllFunctions()
{
	std::map<std::string, double> results;
	for (const auto& name : m_functions | std::views::keys)
	{
		results[name] = GetCalculatedFunctionValue(name);
	}

	return results;
}

bool Calculator::Exists(const std::string& id) const
{
	return m_variables.contains(id) || m_functions.contains(id);
}

void Calculator::ClearCache()
{
	m_cache.clear();
}

double Calculator::GetCalculatedFunctionValue(const std::string& rootId)
{
	if (m_cache.contains(rootId))
	{
		return m_cache[rootId];
	}

	if (m_variables.contains(rootId))
	{
		return m_variables.at(rootId);
	}

	if (!m_functions.contains(rootId))
	{
		throw std::invalid_argument("Function '" + rootId + "' not found");
	}

	FillEvaluationCache(rootId);

	return m_cache.at(rootId);
}

void Calculator::FillEvaluationCache(const std::string& rootId)
{
	std::vector<std::string> evaluationStack;
	evaluationStack.push_back(rootId);

	while (!evaluationStack.empty())
	{
		const std::string& currentId = evaluationStack.back();

		if (m_variables.contains(currentId) || m_cache.contains(currentId))
		{
			evaluationStack.pop_back();
			continue;
		}

		if (!m_functions.contains(currentId))
		{
			throw std::invalid_argument("Function '" + currentId + "' not found during evaluation");
		}

		const auto& function = m_functions.at(currentId);
		bool canCalculate = true;

		if (m_functions.contains(function.left) && !m_cache.contains(function.left))
		{
			evaluationStack.push_back(function.left);
			canCalculate = false;
		}

		if (function.isBinary && m_functions.contains(function.right) && !m_cache.contains(function.right))
		{
			evaluationStack.push_back(function.right);
			canCalculate = false;
		}

		if (canCalculate)
		{
			m_cache[currentId] = ExecuteOperation(function);
			evaluationStack.pop_back();
		}
	}
}

double Calculator::ExecuteOperation(const Function& function)
{
	const double leftVal = GetValueOrNumber(function.left);
	if (!function.isBinary)
	{
		return leftVal;
	}

	const double rightVal = GetValueOrNumber(function.right);

	if (std::isnan(leftVal) || std::isnan(rightVal))
	{
		throw std::runtime_error("Cannot execute operation: one of the operands is NaN");
	}

	switch (function.operation)
	{
	case Operation::Add:
		return leftVal + rightVal;
	case Operation::Sub:
		return leftVal - rightVal;
	case Operation::Mul:
		return leftVal * rightVal;
	case Operation::Div:
		if (rightVal == 0)
		{
			throw std::runtime_error("Division by zero");
		}
		return leftVal / rightVal;
	default:
		throw std::logic_error("Undefined operation");
	}
}

bool Calculator::IsValidVariable(const std::string& id)
{
	if (id.empty())
	{
		return false;
	}

	if (!std::isalpha(static_cast<unsigned char>(id[0])) && id[0] != '_')
	{
		return false;
	}

	for (size_t i = 1; i < id.size(); ++i)
	{
		if (!std::isalnum(static_cast<unsigned char>(id[i])) && id[i] != '_')
		{
			return false;
		}
	}

	return true;
}

bool Calculator::IsOperandValid(const std::string& operand) const
{
	if (Exists(operand))
	{
		return true;
	}

	try
	{
		size_t pos;
		std::stod(operand, &pos);
		return pos == operand.size();
	}
	catch (...)
	{
		return false;
	}
}

double Calculator::GetValueOrNumber(const std::string& idOrNumber)
{
	if (m_variables.contains(idOrNumber) || m_functions.contains(idOrNumber))
	{
		return GetValue(idOrNumber);
	}

	try
	{
		size_t pos;
		const double value = std::stod(idOrNumber, &pos);
		if (pos != idOrNumber.size())
		{
			throw std::invalid_argument("Invalid number: " + idOrNumber);
		}

		return value;
	}
	catch (...)
	{
		throw std::invalid_argument("Operand '" + idOrNumber + "' does not exist");
	}
}