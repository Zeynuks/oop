#pragma once
#include "TemplateStorage.h"
#include "aho_corasick/Trie.hpp"

#include <ranges>
#include <string>

inline std::string ExpandTemplate(const std::string& tpl, const std::unordered_map<std::string, std::string>& templates)
{
	Trie<char> trie;
	trie.RemoveOverlaps();

	for (const auto& key : templates | std::views::keys)
	{
		trie.Insert(key);
	}

	const auto emits = trie.ParseText(tpl);

	std::string result;
	size_t lastPos = 0;

	for (const auto& emit : emits)
	{
		if (emit.GetStart() > lastPos)
		{
			result.append(tpl, lastPos, emit.GetStart() - lastPos);
		}

		result.append(templates.at(emit.GetKeyword()));
		lastPos = emit.GetEnd() + 1;
	}

	if (lastPos < tpl.size())
	{
		result.append(tpl, lastPos, tpl.size() - lastPos);
	}

	return result;
}
