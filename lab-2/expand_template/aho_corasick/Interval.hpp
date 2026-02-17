#pragma once

class Interval
{
public:
	Interval(const size_t start, const size_t end)
		: m_start(start)
		, m_end(end)
	{
	}

	[[nodiscard]] size_t GetStart() const
	{
		return m_start;
	}

	[[nodiscard]] size_t GetEnd() const
	{
		return m_end;
	}

	[[nodiscard]] size_t Size() const
	{
		return m_end - m_start + 1;
	}

	[[nodiscard]] bool OverlapsWith(const Interval& other) const
	{
		return m_start <= other.m_end && m_end >= other.m_start;
	}

	[[nodiscard]] bool OverlapsWith(const size_t point) const
	{
		return m_start <= point && point <= m_end;
	}

	bool operator<(const Interval& other) const
	{
		return GetStart() < other.GetStart();
	}

	bool operator!=(const Interval& other) const
	{
		return GetStart() != other.GetStart() || GetEnd() != other.GetEnd();
	}

	bool operator==(const Interval& other) const
	{
		return GetStart() == other.GetStart() && GetEnd() == other.GetEnd();
	}

private:
	size_t m_start;
	size_t m_end;
};
