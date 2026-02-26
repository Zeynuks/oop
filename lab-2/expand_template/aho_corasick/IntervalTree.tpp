#pragma once

#include <algorithm>
#include <limits>
#include <set>

template <typename T>
class IntervalTree<T>::Node
{
public:
	explicit Node(const IntervalCollection& intervals)
	{
		m_point = DetermineMedian(intervals);

		IntervalCollection leftIntervals;
		IntervalCollection rightIntervals;

		for (const auto& interval : intervals)
		{
			if (interval.GetEnd() < m_point)
			{
				leftIntervals.push_back(interval);
			}
			else if (interval.GetStart() > m_point)
			{
				rightIntervals.push_back(interval);
			}
			else
			{
				m_intervals.push_back(interval);
			}
		}

		if (!leftIntervals.empty())
		{
			m_left = std::make_unique<Node>(leftIntervals);
		}

		if (!rightIntervals.empty())
		{
			m_right = std::make_unique<Node>(rightIntervals);
		}
	}

	IntervalCollection FindOverlaps(const T& interval)
	{
		IntervalCollection overlaps;

		if (m_point < interval.GetStart())
		{
			AddOverlaps(interval, overlaps, FindOverlapsInNode(m_right, interval));
			AddOverlaps(interval, overlaps, CheckRightOverlaps(interval));
		}
		else if (m_point > interval.GetEnd())
		{
			AddOverlaps(interval, overlaps, FindOverlapsInNode(m_left, interval));
			AddOverlaps(interval, overlaps, CheckLeftOverlaps(interval));
		}
		else
		{
			AddOverlaps(interval, overlaps, m_intervals);
			AddOverlaps(interval, overlaps, FindOverlapsInNode(m_left, interval));
			AddOverlaps(interval, overlaps, FindOverlapsInNode(m_right, interval));
		}

		return overlaps;
	}

private:
	size_t m_point;
	std::unique_ptr<Node> m_left;
	std::unique_ptr<Node> m_right;
	IntervalCollection m_intervals;

	static size_t DetermineMedian(const IntervalCollection& intervals)
	{
		size_t start = std::numeric_limits<size_t>::max();
		size_t end = std::numeric_limits<size_t>::max();

		for (const auto& interval : intervals)
		{
			if (start == std::numeric_limits<size_t>::max() || interval.GetStart() < start)
			{
				start = interval.GetStart();
			}

			if (end == std::numeric_limits<size_t>::max() || interval.GetEnd() > end)
			{
				end = interval.GetEnd();
			}
		}

		return (start + end) / 2;
	}

	IntervalCollection CheckLeftOverlaps(const T& interval) const
	{
		IntervalCollection overlaps;
		for (const auto& cur : m_intervals)
		{
			if (cur.GetStart() <= interval.GetEnd())
			{
				overlaps.push_back(cur);
			}
		}

		return overlaps;
	}

	IntervalCollection CheckRightOverlaps(const T& interval) const
	{
		IntervalCollection overlaps;
		for (const auto& cur : m_intervals)
		{
			if (cur.GetEnd() >= interval.GetStart())
			{
				overlaps.push_back(cur);
			}
		}

		return overlaps;
	}

	static void AddOverlaps(const T& interval, IntervalCollection& collection, const IntervalCollection& newOverlaps)
	{
		for (const auto& i : newOverlaps)
		{
			if (i != interval)
			{
				collection.push_back(i);
			}
		}
	}

	static IntervalCollection FindOverlapsInNode(const std::unique_ptr<Node>& node, const T& interval)
	{
		if (node)
		{
			return node->FindOverlaps(interval);
		}

		return IntervalCollection();
	}
};

template <typename T>
IntervalTree<T>::IntervalTree(const IntervalCollection& intervals)
{
	m_root = std::make_unique<Node>(intervals);
}

template <typename T>
IntervalTree<T>::IntervalCollection IntervalTree<T>::FindOverlaps(const T& interval)
{
	if (m_root)
	{
		return m_root->FindOverlaps(interval);
	}

	return IntervalCollection();
}

template <typename T>
IntervalTree<T>::IntervalCollection IntervalTree<T>::RemoveOverlaps(const IntervalCollection& intervals)
{
	IntervalCollection result(intervals.begin(), intervals.end());

	std::sort(result.begin(), result.end(), [](const T& a, const T& b) {
		if (b.Size() - a.Size() == 0)
		{
			return a.GetStart() > b.GetStart();
		}

		return a.Size() > b.Size();
	});

	std::set<T> removeSet;

	for (const auto& interval : result)
	{
		if (removeSet.contains(interval))
		{
			continue;
		}

		for (auto overlaps = FindOverlaps(interval); const auto& overlap : overlaps)
		{
			removeSet.insert(overlap);
		}
	}

	for (const auto& interval : removeSet)
	{
		result.erase(std::find(result.begin(), result.end(), interval));
	}

	std::sort(result.begin(), result.end(), [](const T& a, const T& b) {
		return a.GetStart() < b.GetStart();
	});

	return result;
}
