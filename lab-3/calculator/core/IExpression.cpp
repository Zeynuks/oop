#include "IExpression.hpp"
#include <stdexcept>

NumberExpression::NumberExpression(const double value)
	: m_value(value)
{
}

std::vector<std::string> NumberExpression::GetOperands() const
{
	return {};
}

double NumberExpression::Calculate(const std::vector<double>&) const
{
	return m_value;
}

VariableExpression::VariableExpression(std::string name)
	: m_id(std::move(name))
{
}

std::vector<std::string> VariableExpression::GetOperands() const
{
	return { m_id };
}

double VariableExpression::Calculate(const std::vector<double>& values) const
{
	return values[0];
}

BinaryExpression::BinaryExpression(const Operation operation, std::string left, std::string right)
	: m_operation(operation)
	, m_leftId(std::move(left))
	, m_rightId(std::move(right))
{
}

std::vector<std::string> BinaryExpression::GetOperands() const
{
	return { m_leftId, m_rightId };
}

double BinaryExpression::Calculate(const std::vector<double>& values) const
{
	const double left = values[0];
	const double right = values[1];
	switch (m_operation)
	{
	case Operation::Add:
		return left + right;
	case Operation::Sub:
		return left - right;
	case Operation::Mul:
		return left * right;
	case Operation::Div:
		if (right == 0)
		{
			throw std::runtime_error("Division by zero");
		}

		return left / right;
	default:
		throw std::logic_error("Unknown operation");
	}
}