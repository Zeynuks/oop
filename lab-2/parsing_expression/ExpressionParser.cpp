#include "ExpressionParser.h"
#include "Stack.hpp"
#include <cctype>
#include <optional>
#include <stdexcept>

constexpr char OP_ADD = '+';
constexpr char OP_SUB = '-';
constexpr char OP_MUL = '*';
constexpr char BRACKET_OPEN = '(';
constexpr char BRACKET_CLOSE = ')';

void SkipSpaces(const std::string& line, std::size_t& i)
{
	while (i < line.size() && std::isspace(line[i]))
	{
		++i;
	}
}

long long ParseNumber(const std::string& line, std::size_t& i)
{
	const std::size_t start = i;

	if (line[i] == OP_SUB)
	{
		++i;
	}

	if (i >= line.size() || !std::isdigit(line[i]))
	{
		throw std::runtime_error("Invalid number");
	}

	while (i < line.size() && std::isdigit(line[i]))
	{
		++i;
	}

	return std::stoll(line.substr(start, i - start));
}

void AddArgument(Expression& expression, const long long value)
{
	if (!expression.has_argument)
	{
		expression.value = value;
		expression.has_argument = true;
		return;
	}

	if (expression.operation == OP_ADD)
	{
		expression.value += value;
	}
	else if (expression.operation == OP_MUL)
	{
		expression.value *= value;
	}
	else
	{
		throw std::runtime_error("Operation not set");
	}
}

void HandleOpenBracket(Stack<Expression>& stack, std::size_t& i)
{
	stack.Push(Expression{});
	++i;
}

void HandleOperation(Stack<Expression>& stack, const char op, std::size_t& i)
{
	if (stack.Empty() || stack.Top().operation != 0)
	{
		throw std::runtime_error("Invalid operation position");
	}

	stack.Top().operation = op;
	if (op == OP_MUL)
	{
		stack.Top().value = 1;
	}

	++i;
}

void HandleNumber(Stack<Expression>& stack, const std::string& line, std::size_t& i)
{
	if (stack.Empty())
	{
		throw std::runtime_error("Number outside expression");
	}

	const long long number = ParseNumber(line, i);
	AddArgument(stack.Top(), number);
}

std::optional<long long> HandleCloseBracket(Stack<Expression>& stack, std::size_t& i, const std::string& line)
{
	if (stack.Empty())
	{
		throw std::runtime_error("Unexpected ')'");
	}

	Expression finished = stack.Top();
	stack.Pop();

	if (!finished.has_argument)
	{
		throw std::runtime_error("Expression without arguments");
	}

	if (stack.Empty())
	{
		++i;
		SkipSpaces(line, i);
		if (i != line.size())
		{
			throw std::runtime_error("Extra characters after expression");
		}

		return finished.value;
	}

	AddArgument(stack.Top(), finished.value);
	++i;

	return std::nullopt;
}

std::optional<long long> HandleToken(Stack<Expression>& stack, const std::string& line, std::size_t& i)
{
	SkipSpaces(line, i);

	if (i >= line.size())
	{
		return std::nullopt;
	}

	const char ch = line[i];

	if (std::isdigit(ch) || ch == OP_SUB)
	{
		HandleNumber(stack, line, i);
		return std::nullopt;
	}

	switch (ch)
	{
	case BRACKET_OPEN:
	{
		HandleOpenBracket(stack, i);
		return std::nullopt;
	}
	case OP_ADD:
	case OP_MUL:
	{
		HandleOperation(stack, ch, i);
		return std::nullopt;
	}
	case BRACKET_CLOSE:
	{
		return HandleCloseBracket(stack, i, line);
	}
	default:
	{
		throw std::runtime_error("Invalid character");
	}
	}
}

int64_t ParseExpression(const std::string& line)
{
	Stack<Expression> stack;
	std::size_t i = 0;

	while (i < line.size())
	{
		if (auto result = HandleToken(stack, line, i); result.has_value())
		{
			return result.value();
		}
	}

	if (!stack.Empty())
	{
		throw std::runtime_error("Incomplete expression");
	}

	throw std::runtime_error("Empty expression");
}