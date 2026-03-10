#pragma once

#include "IValueProvider.hpp"
#include "Operations.hpp"

#include <memory>
#include <stdexcept>
#include <string>

class IExpression
{
public:
	virtual ~IExpression() = default;
	virtual double Evaluate(IValueProvider& provider) const = 0;
	virtual void GetDependencies(std::vector<std::string>& deps) const = 0;
};

class NumberExpression : public IExpression
{
public:
	explicit NumberExpression(const double value)
		: m_value(value)
	{
	}

	double Evaluate(IValueProvider& provider) const override
	{
		return m_value;
	}

	void GetDependencies(std::vector<std::string>& deps) const override
	{
	}

private:
	double m_value;
};

class VariableExpression : public IExpression
{
public:
	explicit VariableExpression(std::string id)
		: m_id(std::move(id))
	{
	}

	double Evaluate(IValueProvider& provider) const override
	{
		return provider.GetValue(m_id);
	}

	void GetDependencies(std::vector<std::string>& deps) const override
	{
		deps.push_back(m_id);
	}

private:
	std::string m_id;
};

class BinaryExpression : public IExpression
{
public:
	BinaryExpression(
		const Operation operation,
		std::unique_ptr<IExpression> left,
		std::unique_ptr<IExpression> right)
		: m_operation(operation)
		, m_left(std::move(left))
		, m_right(std::move(right))
	{
	}

	double Evaluate(IValueProvider& provider) const override
	{
		const double left = m_left->Evaluate(provider);
		const double right = m_right->Evaluate(provider);

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

	void GetDependencies(std::vector<std::string>& deps) const override
	{
		m_left->GetDependencies(deps);
		m_right->GetDependencies(deps);
	}

private:
	Operation m_operation;
	std::unique_ptr<IExpression> m_left;
	std::unique_ptr<IExpression> m_right;
};