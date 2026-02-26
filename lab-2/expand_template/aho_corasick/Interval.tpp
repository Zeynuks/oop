#pragma once

inline Interval::Interval(const size_t start, const size_t end)
	: m_start(start)
	, m_end(end)
{
}

inline size_t Interval::GetStart() const
{
	return m_start;
}

inline size_t Interval::GetEnd() const
{
	return m_end;
}

inline size_t Interval::Size() const
{
	return m_end - m_start + 1;
}

inline bool Interval::OverlapsWith(const Interval& other) const
{
	if (m_start <= other.m_end && m_end >= other.m_start)
	{
		return true;
	}

	return false;
}

inline bool Interval::OverlapsWith(const size_t point) const
{
	if (m_start <= point && point <= m_end)
	{
		return true;
	}

	return false;
}

inline bool Interval::operator<(const Interval& other) const
{
	return GetStart() < other.GetStart();
}

inline bool Interval::operator!=(const Interval& other) const
{
	if (GetStart() != other.GetStart() || GetEnd() != other.GetEnd())
	{
		return true;
	}

	return false;
}

inline bool Interval::operator==(const Interval& other) const
{
	if (GetStart() == other.GetStart() && GetEnd() == other.GetEnd())
	{
		return true;
	}

	return false;
}