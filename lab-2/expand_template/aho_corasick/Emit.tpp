#pragma once

template <typename CharType>
Emit<CharType>::Emit()
	: Interval(-1, -1)
	, m_keyword()
	, m_index(0)
{
}

template <typename CharType>
Emit<CharType>::Emit(const size_t start, const size_t end, const StringType& keyword, const unsigned index)
	: Interval(start, end)
	, m_keyword(keyword)
	, m_index(index)
{
}

template <typename CharType>
Emit<CharType>::StringType Emit<CharType>::GetKeyword() const
{
	return m_keyword;
}

template <typename CharType>
unsigned Emit<CharType>::GetIndex() const
{
	return m_index;
}

template <typename CharType>
bool Emit<CharType>::IsEmpty() const
{
	if (GetStart() == static_cast<size_t>(-1) && GetEnd() == static_cast<size_t>(-1))
	{
		return true;
	}

	return false;
}