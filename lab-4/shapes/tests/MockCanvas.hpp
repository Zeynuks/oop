#pragma once

#include "../core/Color.hpp"
#include "../core/ICanvas.hpp"
#include "../core/Point.hpp"

#include <gmock/gmock.h>
#include <vector>

class MockCanvas : public ICanvas
{
public:
	explicit MockCanvas()
	{
	}

	MOCK_METHOD(void, DrawLine, (const Point from, const Point to, const Color outlineColor), (override));
	MOCK_METHOD(void, DrawPolygon, (const std::vector<Point>& points, const Color outlineColor), (override));
	MOCK_METHOD(void, FillPolygon, (const std::vector<Point>& points, const Color fillColor), (override));
	MOCK_METHOD(void, DrawCircle, (const Point center, const double radius, const Color outlineColor), (override));
	MOCK_METHOD(void, FillCircle, (const Point center, const double radius, const Color fillColor), (override));
};