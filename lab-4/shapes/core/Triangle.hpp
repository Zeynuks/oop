#pragma once

#include "BaseShape.hpp"
#include "Point.hpp"

#include <format>

class Triangle : public BaseSolidShape
{
public:
	Triangle(const Point& m_p1, const Point& m_p2, const Point& m_p3, const Color& outlineColor, const Color& fillColor)
		: BaseShape(outlineColor)
		, BaseSolidShape(outlineColor, fillColor)
		, m_p1(m_p1)
		, m_p2(m_p2)
		, m_p3(m_p3)
	{
	}

	double GetArea() override
	{
		const double b = m_p1.DistanceTo(m_p2);
		const double c = m_p2.DistanceTo(m_p3);
		const double a = m_p3.DistanceTo(m_p1);

		const double perimeter = (a + b + c) / 2;

		return std::sqrt(perimeter * (perimeter - a) * (perimeter - b) * (perimeter - c));
	}

	double GetPerimeter() override
	{
		const double b = m_p1.DistanceTo(m_p2);
		const double c = m_p2.DistanceTo(m_p3);
		const double a = m_p3.DistanceTo(m_p1);

		return a + b + c;
	}

	std::string ToString() override
	{
		return std::format(
			"Triangle:\n"
			"\tPoint 1: {}\n"
			"\tPoint 2: {}\n"
			"\tPoint 3: {}\n",
			m_p1, m_p2, m_p3);
	}

	void Draw(ICanvas& canvas) const override
	{
		canvas.FillPolygon({ m_p1, m_p2, m_p3 }, GetFillColor());
		canvas.DrawPolygon({ m_p1, m_p2, m_p3 }, GetOutlineColor());
	}

private:
	Point m_p1;
	Point m_p2;
	Point m_p3;
};