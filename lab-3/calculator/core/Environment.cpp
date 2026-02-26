#include "Environment.hpp"
#include <algorithm>

void Environment::SetVariable(const std::string& id, const double val)
{
	m_variables[id] = val;
	ClearCache();
}

void Environment::SetFunction(const std::string& id, std::shared_ptr<IExpression> expr)
{
	m_functions[id] = std::move(expr);
	ClearCache();
}

void Environment::ClearCache()
{
	m_cache.clear();
}

bool Environment::IsVariable(const std::string& id) const
{
	return m_variables.contains(id);
}

bool Environment::IsFunction(const std::string& id) const
{
	return m_functions.contains(id);
}

bool Environment::Exists(const std::string& id) const
{
	return IsVariable(id) || IsFunction(id);
}

double Environment::GetValue(const std::string& id)
{
	if (IsVariable(id))
	{
		return m_variables.at(id);
	}

	if (IsFunction(id))
	{
		if (!m_cache.contains(id))
		{
			FillEvaluationCache(id);
		}

		return m_cache.at(id);
	}

	return std::stod(id);
}

void Environment::FillEvaluationCache(const std::string& rootId)
{
	std::vector<std::string> stack;
	stack.push_back(rootId);

	while (!stack.empty())
	{
		const std::string& curr = stack.back();

		if (m_variables.contains(curr) || m_cache.contains(curr))
		{
			stack.pop_back();
			continue;
		}

		auto it = m_functions.find(curr);
		if (it == m_functions.end())
		{
			stack.pop_back();
			continue;
		}

		const auto& expr = it->second;
		auto operands = expr->GetOperands();
		bool ready = true;

		for (const auto& operation : operands)
		{
			if (IsFunction(operation) && !m_cache.contains(operation))
			{
				stack.push_back(operation);
				ready = false;
			}
		}

		if (ready)
		{
			std::vector<double> vals;
			for (const auto& operation : operands)
			{
				vals.push_back(GetValue(operation));
			}

			m_cache[curr] = expr->Calculate(vals);
			stack.pop_back();
		}
	}
}

std::unordered_map<std::string, double> Environment::GetAllVariables() const
{
	return m_variables;
}

std::unordered_map<std::string, std::shared_ptr<IExpression>> Environment::GetAllFunctions() const
{
	return m_functions;
}