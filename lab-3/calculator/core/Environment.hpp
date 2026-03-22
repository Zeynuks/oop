#pragma once

#include "Component.hpp"

#include <map>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Environment
{
public:
	bool Exists(const std::string& id) const
	{
		return m_cells.contains(id);
	}

	bool IsVariable(const std::string& id) const
	{
		return m_vars.contains(id);
	}

	bool IsFunction(const std::string& id) const
	{
		return m_fns.contains(id);
	}

	void DeclareVariable(const std::string& id)
	{
		m_cells.emplace(id, Cell(Number{ std::numeric_limits<double>::quiet_NaN() }));
		m_vars.insert(id);
	}

	void SetVariable(const std::string& id, const double value)
	{
		if (!Exists(id))
		{
			m_cells.emplace(id, Cell(Number{ value }));
			m_vars.insert(id);
		}
		else
		{
			m_cells.at(id).SetValue(Number{ value });
		}

		InvalidateDependents(id);
	}

	void DeclareFunction(const std::string& id, Component expr)
	{
		DependencyScanner scanner;
		std::visit(scanner, expr);

		m_cells.emplace(id, Cell(std::move(expr)));
		m_fns.insert(id);

		for (const auto& depName : scanner.deps)
		{
			m_reverseDeps[depName].push_back(id);
		}
	}

	double GetValue(const std::string& id) const
	{
		const auto it = m_cells.find(id);
		if (it == m_cells.end())
		{
			return std::numeric_limits<double>::quiet_NaN();
		}

		auto resolve = std::bind_front(&Environment::GetValue, this);
		return it->second.GetValue(resolve);
	}

	const std::map<std::string, Cell>& GetCells() const
	{
		return m_cells;
	}

	std::vector<std::pair<std::string, double>> GetAllVariables() const
	{
		std::vector<std::pair<std::string, double>> result;

		for (const auto& name : m_cells | std::views::keys)
		{
			if (IsVariable(name))
			{
				result.push_back({ name, GetValue(name) });
			}
		}

		return result;
	}

	std::vector<std::pair<std::string, double>> GetAllFunctions() const
	{
		std::vector<std::pair<std::string, double>> result;

		for (const auto& name : m_cells | std::views::keys)
		{
			if (IsFunction(name))
			{
				result.push_back({ name, GetValue(name) });
			}
		}

		return result;
	}

private:
	std::map<std::string, Cell> m_cells;
	std::unordered_set<std::string> m_vars;
	std::unordered_set<std::string> m_fns;

	std::unordered_map<std::string, std::vector<std::string>> m_reverseDeps;

	void InvalidateDependents(const std::string& id)
	{
		if (!m_reverseDeps.contains(id))
		{
			return;
		}

		for (const auto& dependentId : m_reverseDeps.at(id))
		{
			if (auto& cell = m_cells.at(dependentId); cell.IsCached())
			{
				cell.InvalidateCache();
				InvalidateDependents(dependentId);
			}
		}
	}
};