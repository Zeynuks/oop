#pragma once

#include "Color.hpp"
#include "Point.hpp"
#include <vector>

class ICanvas
{
public:
	virtual ~ICanvas() = default;

	virtual void DrawLine(Point from, Point to, Color color) = 0;
	virtual void DrawPolygon(const std::vector<Point>& points, Color color) = 0;
	virtual void FillPolygon(const std::vector<Point>& points, Color color) = 0;
	virtual void DrawCircle(Point center, double radius, Color color) = 0;
	virtual void FillCircle(Point center, double radius, Color color) = 0;
};