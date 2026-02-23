#pragma once
#include "Environment.hpp"

class Calculator
{
public:
	struct Function
	{
		std::string left;
		std::string right;
		Operation operation = Operation::None;
	};

	void DeclareVariable(const std::string& id);
	void AssignVariable(const std::string& id, double value);
	void AssignVariable(const std::string& id1, const std::string& id2);
	void DefineFunction(const std::string& id, const std::string& operand);
	void DefineFunction(const std::string& id, const Function& function);

	double GetValue(const std::string& id);
	const std::unordered_map<std::string, double>& GetAllVariables() const;
	std::unordered_map<std::string, double> GetAllFunctions();
	bool Exists(const std::string& id) const;

private:
	Environment m_env;
	static Operation MapOperation(Operation operation);
	static bool IsValidVariable(const std::string& id);
	bool IsOperandValid(const std::string& op) const;
};