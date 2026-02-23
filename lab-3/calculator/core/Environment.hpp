#pragma once
#include "IExpression.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Environment
{
public:
	void SetVariable(const std::string& id, double val);
	void SetFunction(const std::string& id, std::shared_ptr<IExpression> expr);
	void ClearCache();
	bool IsVariable(const std::string& id) const;
	bool IsFunction(const std::string& id) const;
	bool Exists(const std::string& id) const;

	double GetValue(const std::string& id);
	const std::unordered_map<std::string, double>& GetAllVariables() const;
	const std::unordered_map<std::string, std::shared_ptr<IExpression>>& GetAllFunctions() const;

private:
	void FillEvaluationCache(const std::string& rootId);

	std::unordered_map<std::string, double> m_variables;
	std::unordered_map<std::string, std::shared_ptr<IExpression>> m_functions;
	std::unordered_map<std::string, double> m_cache;
};