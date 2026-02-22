#pragma once

#include <string>
#include <vector>

class ICommand
{
public:
	virtual void ParseArgs(const std::vector<std::string>&) {}
	virtual void Execute() = 0;
	virtual ~ICommand() = default;
};