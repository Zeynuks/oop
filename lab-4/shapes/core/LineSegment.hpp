#pragma once

#include "BaseShape.hpp"
#include "Point.hpp"

class LineSegment : public BaseShape
{
public:
	LineSegment(const Point& m_start_point, const Point& m_end_point, const Color& outlineColor)
		: BaseShape(outlineColor)
		, m_startPoint(m_start_point)
		, m_endPoint(m_end_point)
	{
	}

	double GetArea() override
	{
		return 0;
	}

	double GetPerimeter() override
	{
		return m_startPoint.DistanceTo(m_endPoint);
	}

	std::string ToString() override
	{
		return std::format(
				   "LineSegment:\n"
				   "\tStartPoint: {}\n"
				   "\tEndPoint: {}\n"
				   "\tLength: {:.2f}\n",
				   m_startPoint,
				   m_endPoint,
				   m_startPoint.DistanceTo(m_endPoint))
			+ BaseShape::ToString();
	}

	Point GetStartPoint() const
	{
		return m_startPoint;
	}

	Point GetEndPoint() const
	{
		return m_endPoint;
	}

	void Draw(ICanvas& canvas) const override
	{
		canvas.DrawLine(m_startPoint, m_endPoint, GetOutlineColor());
	}

private:
	Point m_startPoint;
	Point m_endPoint;
};