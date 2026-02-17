#pragma once

#include "Interval.hpp"
#include <string>

template <typename CharType>
class Emit : public Interval
{
public:
	using StringType = std::basic_string<CharType>;

	Emit()
		: Interval(-1, -1)
		, m_keyword()
		, m_index(0)
	{
	}

	Emit(const size_t start, const size_t end, const StringType& keyword, const unsigned index)
		: Interval(start, end)
		, m_keyword(keyword)
		, m_index(index)
	{
	}

	StringType GetKeyword() const
	{
		return m_keyword;
	}

	[[nodiscard]] unsigned GetIndex() const
	{
		return m_index;
	}

	[[nodiscard]] bool IsEmpty() const
	{
		return GetStart() == -1 && GetEnd() == -1;
	}

private:
	StringType m_keyword;
	unsigned m_index;
};
