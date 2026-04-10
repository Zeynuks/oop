#pragma once

#include "BaseShape.hpp"
#include "Point.hpp"

#include <format>

class Rectangle : public BaseSolidShape
{
public:
	Rectangle(const Point& startPoint, const Point& endPoint, const Color& outlineColor, const Color& fillColor)
		: BaseShape(outlineColor)
		, BaseSolidShape(outlineColor, fillColor)
		, m_startPoint(startPoint)
		, m_endPoint(endPoint)
	{
	}

	double GetArea() override
	{
		return GetHeight() * GetWidth();
	}

	double GetPerimeter() override
	{
		return (GetHeight() + GetWidth()) * 2;
	}

	std::string ToString() override
	{
		return std::format(
				   "Rectangle:\n"
				   "\tWidth: {:.2f}\n"
				   "\tHeight: {:.2f}\n",
				   GetWidth(), GetHeight())
			+ BaseSolidShape::ToString();
	}

	double GetWidth() const
	{
		return std::abs(m_startPoint.x - m_endPoint.x);
	}

	double GetHeight() const
	{
		return std::abs(m_startPoint.y - m_endPoint.y);
	}

	void Draw(ICanvas& canvas) const override
	{
		const std::vector vertices = {
			m_startPoint,
			{ m_endPoint.x, m_startPoint.y },
			m_endPoint,
			{ m_startPoint.x, m_endPoint.y }
		};

		canvas.FillPolygon(vertices, GetFillColor());
		canvas.DrawPolygon(vertices, GetOutlineColor());
	}

private:
	Point m_startPoint;
	Point m_endPoint;
};