#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

template <typename CharType>
class State
{
public:
	using Ptr = State*;
	using UniquePtr = std::unique_ptr<State>;
	using StringType = std::basic_string<CharType>;
	using KeyIndex = std::pair<StringType, unsigned>;
	using EmitCollection = std::set<KeyIndex>;
	using StateCollection = std::vector<Ptr>;
	using TransitionCollection = std::vector<CharType>;

	State()
		: State(0)
	{
	}

	explicit State(const size_t depth)
		: m_depth(depth)
		, m_root(depth == 0 ? this : nullptr)
		, m_failure(nullptr)
		, m_emits()
	{
	}

	[[nodiscard]] size_t GetDepth() const
	{
		return m_depth;
	}

	Ptr NextState(CharType c) const
	{
		return NextState(c, false);
	}

	Ptr NextStateIgnoreRoot(CharType c) const
	{
		return NextState(c, true);
	}

	Ptr AddState(CharType c)
	{
		auto next = NextStateIgnoreRoot(c);
		if (!next)
		{
			next = new State(m_depth + 1);
			m_success[c].reset(next);
		}

		return next;
	}

	void AddEmit(const StringType& keyword, unsigned index)
	{
		m_emits.insert(std::make_pair(keyword, index));
	}

	void AddEmit(const EmitCollection& emits)
	{
		for (const auto& emit : emits)
		{
			m_emits.insert(emit);
		}
	}

	EmitCollection GetEmits() const { return m_emits; }
	Ptr Failure() const { return m_failure; }
	void SetFailure(Ptr failState) { m_failure = failState; }

	StateCollection GetStates() const
	{
		StateCollection result;
		for (const auto& pair : m_success)
		{
			result.push_back(pair.second.get());
		}

		return result;
	}

	TransitionCollection GetTransitions() const
	{
		TransitionCollection result;
		for (const auto& pair : m_success)
		{
			result.push_back(pair.first);
		}

		return result;
	}

private:
	size_t m_depth;
	Ptr m_root;
	std::map<CharType, UniquePtr> m_success;
	Ptr m_failure;
	EmitCollection m_emits;

	Ptr NextState(CharType c, const bool ignoreRoot) const
	{
		auto it = m_success.find(c);
		if (it != m_success.end())
		{
			return it->second.get();
		}
		if (!ignoreRoot && m_root)
		{
			return m_root;
		}

		return nullptr;
	}
};
