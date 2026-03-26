#pragma once

#include "Environment.hpp"
#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>

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

		m_env.DeclareVariable(id);
	}

	void AssignVariable(const std::string& id, double value)
	{
		if (m_env.IsFunction(id))
		{
			throw std::invalid_argument("Cannot assign to function: " + id);
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

		Component expr;
		if (IsNumber(operand))
		{
			expr = Number{ std::stod(operand) };
		}
		else
		{
			if (!m_env.Exists(operand))
			{
				throw std::invalid_argument("Invalid operand encountered");
			}
			expr = Variable{ operand };
		}

		m_env.DeclareFunction(id, std::move(expr));
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

		auto func = std::make_unique<::Function>(
			CreateComponent(function.left),
			CreateComponent(function.right),
			function.operation);

		m_env.DeclareFunction(id, std::move(func));
	}

	double GetValue(const std::string& id)
	{
		if (!m_env.Exists(id))
		{
			throw std::invalid_argument("Identifier not found: " + id);
		}

		return m_env.GetValue(id);
	}

	std::vector<std::pair<std::string, double>> GetAllVariables()
	{
		return m_env.GetAllVariables();
	}

	std::vector<std::pair<std::string, double>> GetAllFunctions()
	{
		return m_env.GetAllFunctions();
	}

private:
	Environment m_env;

	Component CreateComponent(const std::string& operand) const
	{
		if (IsNumber(operand))
		{
			return Number{ std::stod(operand) };
		}

		if (!m_env.Exists(operand))
		{
			throw std::invalid_argument("Invalid operand encountered");
		}

		return Variable{ operand };
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
};