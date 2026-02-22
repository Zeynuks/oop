#pragma once

#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <optional>

class Calculator {
public:
	enum class Operation {
		None, Add, Sub, Mul, Div
	};

	struct Function {
		std::string left;
		std::string right;
		Operation operation = Operation::None;
		bool isBinary = false;
	};
	
    bool DeclareVariable(const std::string& id);
    bool AssignVariable(const std::string& id, double value);
    bool AssignVariableFromId(const std::string& id1, const std::string& id2);
    bool DefineFunction(const std::string& id, const std::string& operand);
    bool DefineFunction(const std::string& id, const std::string& left, Operation operation, const std::string& right);

    double GetValue(const std::string& id);
    [[nodiscard]] const std::map<std::string, double>& GetAllVariables() const;
    std::map<std::string, double> GetAllFunctions();
    [[nodiscard]] bool Exists(const std::string& id) const;

private:
    std::map<std::string, double> m_variables;
    std::map<std::string, Function> m_functions;
    std::map<std::string, double> m_cache;

    void ClearCache();
    double GetCalculatedFunctionValue(const std::string& rootId);
	void FillEvaluationCache(const std::string& rootId);
    double ExecuteOperation(const Function& function);
};