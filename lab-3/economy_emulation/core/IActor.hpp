#pragma once

#include <string>

using ActorId = unsigned long long;

class IActor
{
public:
	virtual ~IActor() = default;
	virtual ActorId GetId() const = 0;
	virtual std::string GetName() const = 0;
};
