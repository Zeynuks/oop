#pragma once

#include "IObservable.hpp"
#include "Operations.hpp"

#include <cmath>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <unordered_set>
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
	void operator()(const Number&) const
	{

	}

	void operator()(const Variable& v)
	{
		deps.push_back(v.name);
	}

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

class Cell : public IObservable
	, public IObserver
{
public:
	explicit Cell(Component value)
		: m_value(std::move(value))
		, m_cache(std::numeric_limits<double>::quiet_NaN())
		, m_isCached(false)
	{
	}

	void OnDependencyChanged() override
	{
		if (m_isCached)
		{
			InvalidateCache();
			NotifyObservers();
		}
	}

	void AddObserver(IObserver& obs) override
	{
		const auto it = std::ranges::find_if(m_observers,
			[&obs](auto& ref) {
				return &ref.get() == &obs;
			});

		if (it == m_observers.end())
		{
			m_observers.push_back(obs);
		}
	}

	void RemoveObserver(IObserver& obs) override
	{
		std::erase_if(m_observers, [&obs](auto& ref) {
			return &ref.get() == &obs;
		});
	}

	void NotifyObservers() override
	{
		for (auto& obs : m_observers)
		{
			obs.get().OnDependencyChanged();
		}
	}

	double GetValue(const std::function<double(const std::string&)>& resolve)
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
		NotifyObservers();
	}

	void InvalidateCache()
	{
		m_isCached = false;
		m_cache = std::numeric_limits<double>::quiet_NaN();
	}

	bool IsCached() const
	{
		return m_isCached;
	}

private:
	Component m_value;
	double m_cache;
	bool m_isCached;
	std::vector<std::reference_wrapper<IObserver>> m_observers;
};