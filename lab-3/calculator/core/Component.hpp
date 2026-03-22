#pragma once

#include "Operations.hpp"
#include <cmath>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <variant>
#include <vector>

struct Number;
struct Variable;
struct Function;

using Component = std::variant<Number, Variable, std::unique_ptr<Function>>;

struct Number
{
	double value;
};

struct Variable
{
	std::string name;
};

struct Function
{
	Component lhs;
	Component rhs;
	Operation operation;
};

struct Evaluator
{
	std::function<double(const std::string&)> resolve;

	double operator()(const Number& number) const
	{
		return number.value;
	}

	double operator()(const Variable& variable) const
	{
		return resolve ? resolve(variable.name) : std::numeric_limits<double>::quiet_NaN();
	}

	double operator()(const std::unique_ptr<Function>& function) const
	{
		if (!function)
		{
			return 0.0;
		}

		const double left = std::visit(*this, function->lhs);
		const double right = std::visit(*this, function->rhs);

		if (std::isnan(left) || std::isnan(right))
		{
			return std::numeric_limits<double>::quiet_NaN();
		}

		switch (function->operation)
		{
		case Operation::Add:
			return left + right;
		case Operation::Sub:
			return left - right;
		case Operation::Mul:
			return left * right;
		case Operation::Div:
			return std::abs(right) < 1e-9 ? std::numeric_limits<double>::quiet_NaN() : left / right;
		default:
			return std::numeric_limits<double>::quiet_NaN();
		}
	}
};

struct DependencyScanner
{
	void operator()(const Number&) const {}
	void operator()(const Variable& v) { deps.push_back(v.name); }
	void operator()(const std::unique_ptr<Function>& f)
	{
		if (!f)
		{
			return;
		}
		std::visit(*this, f->lhs);
		std::visit(*this, f->rhs);
	}
	std::vector<std::string> deps;
};

class Cell
{
public:
	explicit Cell(Component value)
		: m_value(std::move(value))
		, m_cache(std::numeric_limits<double>::quiet_NaN())
		, m_isCached(false)
	{
	}

	double GetValue(const std::function<double(const std::string&)>& resolve) const
	{
		if (m_isCached)
		{
			return m_cache;
		}

		const double result = std::visit(Evaluator{ resolve }, m_value);
		m_cache = result;
		m_isCached = true;
		return m_cache;
	}

	void SetValue(Component value)
	{
		m_value = std::move(value);
		InvalidateCache();
	}

	void InvalidateCache() const
	{
		m_isCached = false;
		m_cache = std::numeric_limits<double>::quiet_NaN();
	}

	bool IsCached() const { return m_isCached; }

private:
	Component m_value;
	mutable double m_cache;
	mutable bool m_isCached;
};