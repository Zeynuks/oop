#pragma once

#include "Emit.hpp"
#include <string>

template <typename CharType>
class Token
{
public:
	enum Type
	{
		Fragment,
		Match
	};

	using StringType = std::basic_string<CharType>;
	using EmitType = Emit<CharType>;

	explicit Token(StringType fragment)
		: m_type(Fragment)
		, m_fragment(fragment)
		, m_emit()
	{
	}

	Token(StringType fragment, const EmitType& emit)
		: m_type(Match)
		, m_fragment(fragment)
		, m_emit(emit)
	{
	}

	[[nodiscard]] bool IsMatch() const
	{
		return m_type == Match;
	}

	StringType GetFragment() const
	{
		return m_fragment;
	}

	EmitType GetEmit() const
	{
		return m_emit;
	}

private:
	Type m_type;
	StringType m_fragment;
	EmitType m_emit;
};
