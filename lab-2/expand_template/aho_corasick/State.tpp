#pragma once

template <typename CharType>
State<CharType>::State()
	: State(0)
{
}

template <typename CharType>
State<CharType>::State(const size_t depth)
	: m_depth(depth)
	, m_root(depth == 0 ? this : nullptr)
	, m_failure(nullptr)
	, m_emits()
{
}

template <typename CharType>
size_t State<CharType>::GetDepth() const
{
	return m_depth;
}

template <typename CharType>
State<CharType>::Ptr State<CharType>::NextState(CharType c) const
{
	return NextState(c, false);
}

template <typename CharType>
State<CharType>::Ptr State<CharType>::NextStateIgnoreRoot(CharType c) const
{
	return NextState(c, true);
}

template <typename CharType>
State<CharType>::Ptr State<CharType>::AddState(CharType c)
{
	auto next = NextStateIgnoreRoot(c);
	if (!next)
	{
		next = new State(m_depth + 1);
		m_success[c].reset(next);
	}

	return next;
}

template <typename CharType>
void State<CharType>::AddEmit(const StringType& keyword, unsigned index)
{
	m_emits.insert(std::make_pair(keyword, index));
}

template <typename CharType>
void State<CharType>::AddEmit(const EmitCollection& emits)
{
	for (const auto& emit : emits)
	{
		m_emits.insert(emit);
	}
}

template <typename CharType>
State<CharType>::EmitCollection State<CharType>::GetEmits() const
{
	return m_emits;
}

template <typename CharType>
State<CharType>::Ptr State<CharType>::Failure() const
{
	return m_failure;
}

template <typename CharType>
void State<CharType>::SetFailure(Ptr failState)
{
	m_failure = failState;
}

template <typename CharType>
State<CharType>::StateCollection State<CharType>::GetStates() const
{
	StateCollection result;
	for (const auto& pair : m_success)
	{
		result.push_back(pair.second.get());
	}

	return result;
}

template <typename CharType>
State<CharType>::TransitionCollection State<CharType>::GetTransitions() const
{
	TransitionCollection result;
	for (const auto& pair : m_success)
	{
		result.push_back(pair.first);
	}

	return result;
}

template <typename CharType>
State<CharType>::Ptr State<CharType>::NextState(CharType c, const bool ignoreRoot) const
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