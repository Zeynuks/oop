#pragma once

#include "./core/Calculator.hpp"
#include <iomanip>
#include <string>
#include <unordered_map>
#include <vector>

namespace CalculatorFunctions
{
namespace Detail
{
inline bool TryParseDouble(const std::string& s, double& out)
{
	try
	{
		size_t pos;
		out = std::stod(s, &pos);
		return pos == s.size();
	}
	catch (...)
	{
		return false;
	}
}

inline Operation StringToOp(const std::string& op)
{
	static const std::unordered_map<std::string, Operation> mapping = {
		{ "+", Operation::Add }, { "-", Operation::Sub },
		{ "*", Operation::Mul }, { "/", Operation::Div }
	};

	if (const auto it = mapping.find(op); it != mapping.end())
	{
		return it->second;
	}

	throw std::invalid_argument("Invalid operation");
}
} // namespace Detail

inline void DeclareVar(Calculator& calc, const std::vector<std::string>& args)
{
	if (args.size() != 1)
	{
		throw std::invalid_argument("Usage: var <id>");
	}
	calc.DeclareVariable(args[0]);
}

inline void AssignVar(Calculator& calc, const std::vector<std::string>& args)
{
	if (args.size() < 3 || args[1] != "=")
	{
		throw std::invalid_argument("Usage: let <id> = <val|id2>");
	}

	if (double value; Detail::TryParseDouble(args[2], value))
	{
		calc.AssignVariable(args[0], value);
	}
	else
	{
		calc.AssignVariable(args[0], args[2]);
	}
}

inline void DefineFunction(Calculator& calc, const std::vector<std::string>& args)
{
	if (args.size() < 3 || args[1] != "=")
	{
		throw std::invalid_argument("Usage: fn <id>=<id1> [op <id2>]");
	}

	if (args.size() == 3)
	{
		calc.DefineFunction(args[0], args[2]);
	}
	else if (args.size() == 5)
	{
		calc.DefineFunction(args[0], { args[2], args[4], Detail::StringToOp(args[3]) });
	}
	else
	{
		throw std::invalid_argument("Invalid function format");
	}
}

inline void Print(Calculator& calc, const std::vector<std::string>& args, std::ostream& out)
{
	if (args.size() != 1)
	{
		throw std::invalid_argument("Usage: print <id>");
	}

	out << std::fixed << std::setprecision(2) << calc.GetValue(args[0]) << std::endl;
}

inline void PrintVars(const Calculator& calc, std::ostream& out)
{
	auto vars = calc.GetAllVariables();
	if (vars.empty())
	{
		throw std::logic_error("No variables");
	}

	for (const auto& [name, val] : vars)
	{
		out << name << ":" << std::fixed << std::setprecision(2) << val << std::endl;
	}
}

inline void PrintFunctions(Calculator& calc, std::ostream& out)
{
	auto fns = calc.GetAllFunctions();
	if (fns.empty())
	{
		throw std::logic_error("No functions");
	}

	for (const auto& [name, val] : fns)
	{
		out << name << ":" << std::fixed << std::setprecision(2) << val << std::endl;
	}
}
} // namespace CalculatorController