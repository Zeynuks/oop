#pragma once

#include "Emit.hpp"
#include "IntervalTree.hpp"
#include "State.hpp"
#include "Token.hpp"

#include <algorithm>
#include <cctype>
#include <queue>
#include <string>
#include <vector>

template <typename CharType>
class Trie
{
public:
	using StringType = std::basic_string<CharType>;
	using TokenType = Token<CharType>;
	using TokenCollection = std::vector<TokenType>;
	using EmitType = Emit<CharType>;
	using EmitCollection = std::vector<EmitType>;
	using StateType = State<CharType>;
	using StatePtr = StateType*;

	class Config
	{
	public:
		Config()
			: m_allowOverlaps(true)
			, m_onlyWholeWords(false)
			, m_caseInsensitive(false)
		{
		}

		[[nodiscard]] bool IsAllowOverlaps() const { return m_allowOverlaps; }
		void SetAllowOverlaps(const bool value)
		{
			m_allowOverlaps = value;
		}

		[[nodiscard]] bool IsOnlyWholeWords() const
		{
			return m_onlyWholeWords;
		}
		void SetOnlyWholeWords(const bool value)
		{
			m_onlyWholeWords = value;
		}

		[[nodiscard]] bool IsCaseInsensitive() const
		{
			return m_caseInsensitive;
		}
		void SetCaseInsensitive(const bool value)
		{
			m_caseInsensitive = value;
		}

	private:
		bool m_allowOverlaps;
		bool m_onlyWholeWords;
		bool m_caseInsensitive;
	};

	Trie()
		: Trie(Config())
	{
	}
	explicit Trie(const Config& cfg)
		: m_root(std::make_unique<StateType>())
		, m_config(cfg)
		, m_failureConstructed(false)
		, m_numKeywords(0)
	{
	}

	Trie& CaseInsensitive()
	{
		m_config.SetCaseInsensitive(true);
		return *this;
	}

	Trie& RemoveOverlaps()
	{
		m_config.SetAllowOverlaps(false);
		return *this;
	}

	Trie& OnlyWholeWords()
	{
		m_config.SetOnlyWholeWords(true);
		return *this;
	}

	void Insert(const StringType& keyword)
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

	TokenCollection Tokenize(const StringType& text)
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

	EmitCollection ParseText(const StringType& text)
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
			IntervalTree<EmitType> tree(EmitCollection(collectedEmits.begin(), collectedEmits.end()));
			auto tmp = tree.RemoveOverlaps(collectedEmits);
			collectedEmits.swap(tmp);
		}

		return collectedEmits;
	}

private:
	std::unique_ptr<StateType> m_root;
	Config m_config;
	bool m_failureConstructed;
	unsigned m_numKeywords;

	void CheckFailureStates()
	{
		if (!m_failureConstructed)
		{
			ConstructFailureStates();
		}
	}

	StatePtr GetState(StatePtr curState, CharType c) const
	{
		StatePtr result = curState->NextState(c);
		while (!result)
		{
			curState = curState->Failure();
			result = curState->NextState(c);
		}
		return result;
	}

	void ConstructFailureStates()
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

	TokenType CreateFragment(const EmitType& emit, const StringType& text, const size_t lastPos) const
	{
		size_t start = lastPos + 1;
		const size_t end = emit.IsEmpty() ? text.size() : emit.GetStart();
		auto len = end - start;
		StringType str = text.substr(start, len);
		return TokenType(str);
	}

	TokenType CreateMatch(const EmitType& emit, const StringType& text) const
	{
		size_t start = emit.GetStart();
		const size_t end = emit.GetEnd() + 1;
		auto len = end - start;
		StringType str = text.substr(start, len);
		return TokenType(str, emit);
	}

	void StoreEmits(size_t pos, StatePtr curState, EmitCollection& collectedEmits) const
	{
		for (auto emits = curState->GetEmits(); const auto& pair : emits)
		{
			StringType keyword = pair.first;
			collectedEmits.push_back(EmitType(pos - keyword.size() + 1, pos, keyword, pair.second));
		}
	}

	void RemovePartialMatches(const StringType& text, EmitCollection& collectedEmits) const
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
};