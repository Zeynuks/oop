#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

class TemplateStorage
{
public:
	std::optional<std::string> Find(const std::string& key) const;
	void AddTemplate(const std::string& key, const std::string& value);
	const std::unordered_map<std::string, std::string>& GetAllTemplates() const;

private:
	std::unordered_map<std::string, std::string> storage;
};