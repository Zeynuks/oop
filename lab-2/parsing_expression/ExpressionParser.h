#pragma once

#include <string>

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

long long ParseExpression(const std::string& line);
