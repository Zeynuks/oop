#pragma once

#include <string>


struct Expression
{
	char operation = 0;
	long long value = 0;
	bool has_argument = false;
};

int64_t ParseExpression(const std::string& line);
