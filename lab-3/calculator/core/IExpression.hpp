#pragma once
#include "Operations.hpp"

#include <memory>
#include <string>
#include <vector>

class IExpression
{
public:
	virtual ~IExpression() = default;
	virtual std::vector<std::string> GetOperands() const = 0;
	virtual double Calculate(const std::vector<double>& operandValues) const = 0;
};

class NumberExpression : public IExpression
{
public:
	explicit NumberExpression(double value);
	std::vector<std::string> GetOperands() const override;
	double Calculate(const std::vector<double>& values) const override;

private:
	double m_value;
};

class VariableExpression : public IExpression
{
public:
	explicit VariableExpression(std::string id);
	std::vector<std::string> GetOperands() const override;
	double Calculate(const std::vector<double>& values) const override;
private:
	std::string m_id;
};

class BinaryExpression : public IExpression
{
public:
	BinaryExpression(Operation operation, std::string left, std::string right);
	std::vector<std::string> GetOperands() const override;
	double Calculate(const std::vector<double>& values) const override;

private:
	Operation m_operation;
	std::string m_leftId;
	std::string m_rightId;
};