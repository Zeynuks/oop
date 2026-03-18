#pragma once

#include "BaseShape.hpp"
#include "Point.hpp"

#include <math.h>

class Circle : public BaseSolidShape
{
public:
	Circle(const double m_radius, const Point& m_center, const Color& outlineColor , const Color& fillColor )
		: BaseShape(outlineColor)
		, BaseSolidShape(outlineColor, fillColor)
		, m_radius(m_radius)
		, m_center(m_center)
	{
	}

	double GetArea() override
	{
		return M_PI * pow(m_radius, 2);
	}

	double GetPerimeter() override
	{
		return 2 * M_PI * m_radius;
	}

	std::string ToString() override
	{
		return std::format(
				   "Circle:\n"
				   "\tCenter: {}\n"
				   "\tRadius: {:.2f}\n",
				   m_center,
				   m_radius)
			+ BaseSolidShape::ToString();
	}

	Point GetCenter() const
	{
		return m_center;
	}

	double GetRadius() const
	{
		return m_radius;
	}

	void Draw(ICanvas& canvas) const override
	{
		canvas.FillCircle(m_center, m_radius, GetFillColor());
		canvas.DrawCircle(m_center, m_radius, GetOutlineColor());
	}

private:
	double m_radius;
	Point m_center;
};