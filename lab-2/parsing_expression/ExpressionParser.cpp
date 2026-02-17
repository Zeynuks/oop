#include "ExpressionParser.h"
#include <cctype>
#include <charconv>
#include <format>
#include <stdexcept>

ExpressionParser::ExpressionParser(const std::string& input)
	: line(input)
{
}

void ExpressionParser::SkipSpaces()
{
	while (!End() && std::isspace(line[pos]))
	{
		++pos;
	}
}

char ExpressionParser::CurrentChar() const
{
	if (End())
	{
		return 0;
	}
	return line[pos];
}

bool ExpressionParser::End() const
{
	return pos >= line.size();
}

long long ExpressionParser::ParseNumber()
{
	const std::size_t start = pos;
	bool negative = false;

	if (line[pos] == OP_SUB)
	{
		negative = true;
		++pos;
	}

	if (End() || !std::isdigit(line[pos]))
	{
		throw std::runtime_error("Invalid number");
	}

	std::size_t end = pos;
	while (end < line.size() && std::isdigit(line[end]))
	{
		++end;
	}

	long long value;
	if (auto [ptr, ec] = std::from_chars(line.data() + start, line.data() + end, value); ec != std::errc())
	{
		throw std::runtime_error("Number parse error at position " + std::to_string(start));
	}

	pos = end;
	return negative ? -value : value;
}

void ExpressionParser::AddArgument(Expression& expression, const long long value)
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

void ExpressionParser::HandleOpenBracket()
{
	stack.Push(Expression{});
	++pos;
}

void ExpressionParser::HandleOperation(const char op)
{
	if (stack.Empty() || stack.Top().operation != 0)
	{
		throw std::runtime_error(std::format("Invalid operation '{}' at position {}", op, pos));
	}

	stack.Top().operation = op;
	if (op == OP_MUL)
	{
		stack.Top().value = 1;
	}

	++pos;
}

void ExpressionParser::HandleNumber()
{
	if (stack.Empty())
	{
		throw std::runtime_error(std::format("Number outside expression at position {}", pos));
	}

	const long long value = ParseNumber();
	AddArgument(stack.Top(), value);
}

std::optional<long long> ExpressionParser::HandleCloseBracket()
{
	if (stack.Empty())
	{
		throw std::runtime_error(std::format("Unexpected ')' at position {}", pos));
	}

	Expression finished = stack.Top();
	stack.Pop();

	if (!finished.has_argument)
	{
		throw std::runtime_error(std::format("Expression without arguments at position {}", pos));
	}

	++pos;
	SkipSpaces();

	if (stack.Empty())
	{
		if (!End())
		{
			throw std::runtime_error(std::format("Extra characters after expression at position {}", pos));
		}
		return finished.value;
	}

	AddArgument(stack.Top(), finished.value);
	return std::nullopt;
}

std::optional<long long> ExpressionParser::HandleToken()
{
	SkipSpaces();
	if (End())
	{
		return std::nullopt;
	}

	char ch = CurrentChar();

	if (std::isdigit(ch) || ch == OP_SUB)
	{
		HandleNumber();
		return std::nullopt;
	}

	switch (ch)
	{
	case BRACKET_OPEN:
		HandleOpenBracket();
		return std::nullopt;
	case OP_ADD:
	case OP_MUL:
		HandleOperation(ch);
		return std::nullopt;
	case BRACKET_CLOSE:
		return HandleCloseBracket();
	default:
		throw std::runtime_error(std::format("Invalid character '{}' at position {}", ch, pos));
	}
}

long long ExpressionParser::Parse()
{
	SkipSpaces();
	if (End())
	{
		throw std::runtime_error("Empty expression");
	}

	while (!End())
	{
		if (auto result = HandleToken(); result.has_value())
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