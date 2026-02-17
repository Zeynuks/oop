#pragma once

#include "IntervalTree.hpp"
#include <algorithm>
#include <cctype>
#include <queue>

template <typename CharType>
Trie<CharType>& Trie<CharType>::CaseInsensitive()
{
	m_config.SetCaseInsensitive(true);

	return *this;
}

template <typename CharType>
Trie<CharType>& Trie<CharType>::RemoveOverlaps()
{
	m_config.SetAllowOverlaps(false);

	return *this;
}

template <typename CharType>
Trie<CharType>& Trie<CharType>::OnlyWholeWords()
{
	m_config.SetOnlyWholeWords(true);

	return *this;
}

template <typename CharType>
void Trie<CharType>::Insert(const StringType& keyword)
{
	if (keyword.empty())
	{
		return;
	}

	StatePtr curState = m_root.get();
	for (auto ch : keyword)
	{
		curState = curState->AddState(ch);
	}
	curState->AddEmit(keyword, m_numKeywords++);
	m_failureConstructed = false;
}

template <typename CharType>
Trie<CharType>::TokenCollection Trie<CharType>::Tokenize(const StringType& text)
{
	TokenCollection tokens;
	auto collectedEmits = ParseText(text);
	size_t lastPos = -1;

	for (const auto& emit : collectedEmits)
	{
		if (emit.GetStart() - lastPos > 1)
		{
			tokens.push_back(CreateFragment(emit, text, lastPos));
		}

		tokens.push_back(CreateMatch(emit, text));
		lastPos = emit.GetEnd();
	}

	if (text.size() - lastPos > 1)
	{
		tokens.push_back(CreateFragment(EmitType(), text, lastPos));
	}

	return tokens;
}

template <typename CharType>
Trie<CharType>::EmitCollection Trie<CharType>::ParseText(const StringType& text)
{
	CheckFailureStates();
	StatePtr curState = m_root.get();
	EmitCollection collectedEmits;
	size_t pos = 0;

	for (auto c : text)
	{
		if (m_config.IsCaseInsensitive())
		{
			c = std::tolower(c);
		}

		curState = GetState(curState, c);
		StoreEmits(pos, curState, collectedEmits);
		pos++;
	}

	if (m_config.IsOnlyWholeWords())
	{
		RemovePartialMatches(text, collectedEmits);
	}

	if (!m_config.IsAllowOverlaps())
	{
		IntervalTree<EmitType> tree(collectedEmits);
		auto tmp = tree.RemoveOverlaps(collectedEmits);
		collectedEmits.swap(tmp);
	}

	return collectedEmits;
}

template <typename CharType>
void Trie<CharType>::CheckFailureStates()
{
	if (!m_failureConstructed)
	{
		ConstructFailureStates();
	}
}

template <typename CharType>
Trie<CharType>::StatePtr Trie<CharType>::GetState(StatePtr curState, CharType c) const
{
	StatePtr result = curState->NextState(c);
	while (!result)
	{
		curState = curState->Failure();
		result = curState->NextState(c);
	}

	return result;
}

template <typename CharType>
void Trie<CharType>::ConstructFailureStates()
{
	std::queue<StatePtr> q;

	for (auto state : m_root->GetStates())
	{
		state->SetFailure(m_root.get());
		q.push(state);
	}

	m_failureConstructed = true;

	while (!q.empty())
	{
		StatePtr curState = q.front();
		q.pop();

		for (auto trans : curState->GetTransitions())
		{
			StatePtr targetState = curState->NextState(trans);
			q.push(targetState);

			StatePtr failState = curState->Failure();
			while (!failState->NextState(trans))
			{
				failState = failState->Failure();
			}

			StatePtr newFail = failState->NextState(trans);
			targetState->SetFailure(newFail);
			targetState->AddEmit(newFail->GetEmits());
		}
	}
}

template <typename CharType>
Trie<CharType>::TokenType Trie<CharType>::CreateFragment(const EmitType& emit, const StringType& text, const size_t lastPos) const
{
	size_t start = lastPos + 1;
	const size_t end = emit.IsEmpty() ? text.size() : emit.GetStart();
	auto len = end - start;
	StringType str = text.substr(start, len);

	return TokenType(str);
}

template <typename CharType>
Trie<CharType>::TokenType Trie<CharType>::CreateMatch(const EmitType& emit, const StringType& text) const
{
	size_t start = emit.GetStart();
	const size_t end = emit.GetEnd() + 1;
	auto len = end - start;
	StringType str = text.substr(start, len);

	return TokenType(str, emit);
}

template <typename CharType>
void Trie<CharType>::StoreEmits(size_t pos, StatePtr curState, EmitCollection& collectedEmits) const
{
	for (auto emits = curState->GetEmits(); const auto& pair : emits)
	{
		StringType keyword = pair.first;
		collectedEmits.push_back(EmitType(pos - keyword.size() + 1, pos, keyword, pair.second));
	}
}

template <typename CharType>
void Trie<CharType>::RemovePartialMatches(const StringType& text, EmitCollection& collectedEmits) const
{
	EmitCollection removeEmits;
	size_t size = text.size();

	for (const auto& emit : collectedEmits)
	{
		if ((emit.GetStart() == 0 || !std::isalpha(text[emit.GetStart() - 1])) && (emit.GetEnd() + 1 == size || !std::isalpha(text[emit.GetEnd() + 1])))
		{
			continue;
		}
		removeEmits.push_back(emit);
	}

	for (auto& emit : removeEmits)
	{
		collectedEmits.erase(std::find(collectedEmits.begin(), collectedEmits.end(), emit));
	}
}