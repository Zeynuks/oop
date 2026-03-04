#pragma once

#include "IMoneyStorage.hpp"

#include <string>
#include <unordered_map>

class EconomyRegistry
{
public:
	static EconomyRegistry& Instance()
	{
		static EconomyRegistry instance;
		return instance;
	}

	void Register(StorageId id, const std::string& ownerName, const std::string& type);
	void Unregister(StorageId id);
	std::string GetName(StorageId id) const;

private:
	std::unordered_map<StorageId, std::string> m_names;
};