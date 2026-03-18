#pragma once

#include "Circle.hpp"
#include "Color.hpp"
#include "IShape.hpp"
#include "LineSegment.hpp"
#include "Point.hpp"
#include "Rectangle.hpp"
#include "Triangle.hpp"

#include <algorithm>
#include <functional>
#include <memory>

class ShapeController
{
public:
	IShape& AddRectangle(const Point& startPoint,
		const Point& endPoint,
		const Color& outlineColor = DefaultColors::Transparent,
		const Color& fillColor = DefaultColors::Red)
	{
		auto rectangle = std::make_unique<Rectangle>(startPoint, endPoint, outlineColor, fillColor);

		m_shapes.push_back(std::move(rectangle));

		return *m_shapes.back();
	}

	IShape& AddLine(
		const Point& start_point,
		const Point& end_point,
		const Color& outlineColor = DefaultColors::Black)
	{
		auto line = std::make_unique<LineSegment>(start_point, end_point, outlineColor);

		m_shapes.push_back(std::move(line));

		return *m_shapes.back();
	}

	IShape& AddCircle(const double radius,
		const Point& center,
		const Color& outlineColor = DefaultColors::Transparent,
		const Color& fillColor = DefaultColors::Green)
	{
		auto circle = std::make_unique<Circle>(radius, center, outlineColor, fillColor);

		m_shapes.push_back(std::move(circle));

		return *m_shapes.back();
	}

	IShape& AddTriangle(const Point& p1,
		const Point& p2,
		const Point& p3,
		const Color& outlineColor = DefaultColors::Transparent,
		const Color& fillColor = DefaultColors::Yellow)
	{
		auto circle = std::make_unique<Triangle>(p1, p2, p3, outlineColor, fillColor);

		m_shapes.push_back(std::move(circle));

		return *m_shapes.back();
	}

	IShape& FindMaxAreaShape() const
	{
		if (m_shapes.empty())
		{
			throw std::runtime_error("No shapes available");
		}

		const auto it = std::ranges::max_element(m_shapes,
			[](const std::unique_ptr<IShape>& a, const std::unique_ptr<IShape>& b) {
				return a->GetArea() < b->GetArea();
			});

		return **it;
	}

	IShape& FindMinPerimeterShape() const
	{
		if (m_shapes.empty())
		{
			throw std::runtime_error("No shapes available");
		}

		const auto it = std::ranges::min_element(m_shapes,
			[](const std::unique_ptr<IShape>& a, const std::unique_ptr<IShape>& b) {
				return a->GetPerimeter() < b->GetPerimeter();
			});

		return **it;
	}

	void DrawShapes(ICanvas& canvas) const
	{
		for (const auto& shape : m_shapes) {
			if (shape) {
				shape->Draw(canvas);
			}
		}
	}

private:
	std::vector<std::unique_ptr<IShape>> m_shapes;
};