#pragma once

#include "Stack.hpp"
#include <string>
#include <optional>

constexpr char OP_ADD = '+';
constexpr char OP_SUB = '-';
constexpr char OP_MUL = '*';
constexpr char BRACKET_OPEN = '(';
constexpr char BRACKET_CLOSE = ')';

struct Expression
{
	char operation = 0;
	long long value = 0;
	bool has_argument = false;
};

class ExpressionParser
{
public:
	explicit ExpressionParser(const std::string& input);

	long long Parse();

private:
	void SkipSpaces();
	char CurrentChar() const;
	bool End() const;

	long long ParseNumber();
	static void AddArgument(Expression& expression, long long value);

	void HandleOpenBracket();
	void HandleOperation(char op);
	void HandleNumber();
	std::optional<long long> HandleCloseBracket();
	std::optional<long long> HandleToken();

	const std::string& line;
	std::size_t pos = 0;
	Stack<Expression> stack;
};
