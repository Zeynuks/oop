#include "EconomyRegistry.hpp"

void EconomyRegistry::Register(const StorageId id, const std::string& ownerName, const std::string& type)
{
	m_names[id] = ownerName + "'s " + type;
}

void EconomyRegistry::Unregister(const StorageId id)
{
	m_names.erase(id);
}

std::string EconomyRegistry::GetName(const StorageId id) const
{
	if (const auto it = m_names.find(id); it != m_names.end()) {
		return it->second;
	}

	return "Deleted/Unknown Storage";
}