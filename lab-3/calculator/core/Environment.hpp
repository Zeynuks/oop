#pragma once

#include <vector>
#include <string>
#include <memory>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

#include "IExpression.hpp"

class Environment : public IValueProvider
{
public:
	void SetVariable(const std::string& id, double value)
	{
		m_variables[id] = value;
		InvalidateCache();
	}

	void SetFunction(const std::string& id, std::unique_ptr<IExpression> expr)
	{
		m_functions[id] = std::move(expr);
		InvalidateCache();
	}

	bool IsVariable(const std::string& id) const
	{
		return m_variables.contains(id);
	}

	bool IsFunction(const std::string& id) const
	{
		return m_functions.contains(id);
	}

	bool Exists(const std::string& id) const
	{
		return IsVariable(id) || IsFunction(id);
	}

	double GetValue(const std::string& id) override
	{
		if (IsVariable(id))
		{
			return m_variables.at(id);
		}

		if (IsFunction(id))
		{
			return EvaluateFunction(id);
		}

		return std::stod(id);
	}

	void InvalidateCache()
	{
		m_cache.clear();
	}

	std::unordered_map<std::string, double> GetAllVariables() const
	{
		return m_variables;
	}

	const std::unordered_map<std::string, std::unique_ptr<IExpression>>& GetAllFunctions() const
	{
		return m_functions;
	}

private:
	double EvaluateFunction(const std::string& id)
	{
		if (const auto it = m_cache.find(id); it != m_cache.end())
		{
			return it->second;
		}

		std::stack<std::pair<std::string, size_t>> evalStack;
		std::unordered_set<std::string> inCurrPath;

		evalStack.push({ id, 0 });

		while (!evalStack.empty())
		{
			if (!EvaluateStack(evalStack, inCurrPath))
			{
				NodeEvaluation(evalStack, inCurrPath);
			}
		}

		return m_cache.at(id);
	}

	bool EvaluateStack(std::stack<std::pair<std::string, size_t>>& stack,
		std::unordered_set<std::string>& path) const
	{
		auto& [currId, depIdx] = stack.top();

		if (depIdx == 0)
		{
			if (path.contains(currId))
			{
				throw std::runtime_error("Cyclic dependency detected: " + currId);
			}

			path.insert(currId);
		}

		std::vector<std::string> deps;
		m_functions.at(currId)->GetDependencies(deps);

		for (; depIdx < deps.size(); ++depIdx)
		{
			if (const std::string& dep = deps[depIdx]; IsFunction(dep) && !m_cache.contains(dep))
			{
				depIdx++;
				stack.push({ dep, 0 });

				return true;
			}
		}

		return false;
	}

	void NodeEvaluation(std::stack<std::pair<std::string, size_t>>& stack,
		std::unordered_set<std::string>& path)
	{
		const std::string currentId = stack.top().first;
		m_cache[currentId] = m_functions.at(currentId)->Evaluate(*this);

		path.erase(currentId);
		stack.pop();
	}

	std::unordered_map<std::string, double> m_variables;
	std::unordered_map<std::string, std::unique_ptr<IExpression>> m_functions;

	std::unordered_map<std::string, double> m_cache;
	std::unordered_set<std::string> m_evaluating;
};