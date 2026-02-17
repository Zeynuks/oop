#pragma once

template <typename CharType>
Token<CharType>::Token(StringType fragment)
	: m_type(Fragment)
	, m_fragment(fragment)
	, m_emit()
{
}

template <typename CharType>
Token<CharType>::Token(StringType fragment, const EmitType& emit)
	: m_type(Match)
	, m_fragment(fragment)
	, m_emit(emit)
{
}

template <typename CharType>
bool Token<CharType>::IsMatch() const
{
	if (m_type == Match)
	{
		return true;
	}

	return false;
}

template <typename CharType>
Token<CharType>::StringType Token<CharType>::GetFragment() const
{
	return m_fragment;
}

template <typename CharType>
Token<CharType>::EmitType Token<CharType>::GetEmit() const
{
	return m_emit;
}
