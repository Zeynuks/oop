#pragma once

#include "../core/Color.hpp"
#include "../core/ICanvas.hpp"
#include "../core/Point.hpp"

#include <format>
#include <iostream>
#include <vector>

class MockCanvas : public ICanvas
{
public:
	explicit MockCanvas(std::ostream& output)
		: m_output(output)
	{
	}

	void DrawLine(const Point from, const Point to, const Color outlineColor) override
	{
		m_output << std::format("DrawLine: from {} to {} color {}\n", from, to, outlineColor);
	}

	void DrawPolygon(const std::vector<Point>& points, const Color outlineColor) override
	{
		m_output << std::format("DrawPolygon: points [{}] outline {}\n", FormatPoints(points), outlineColor);
	}

	void FillPolygon(const std::vector<Point>& points, const Color fillColor) override
	{
		m_output << std::format("FillPolygon: points [{}] fill {}\n", FormatPoints(points), fillColor);
	}

	void DrawCircle(const Point center, const double radius, const Color outlineColor) override
	{
		m_output << std::format("DrawCircle: center {} radius {:.2f} outline {}\n", center, radius, outlineColor);
	}

	void FillCircle(const Point center, const double radius, const Color fillColor) override
	{
		m_output << std::format("FillCircle: center {} radius {:.2f} fill {}\n", center, radius, fillColor);
	}

private:
	std::ostream& m_output;

	static std::string FormatPoints(const std::vector<Point>& points)
	{
		std::string result;
		for (size_t i = 0; i < points.size(); ++i)
		{
			result += std::format("{}{}", points[i], i == points.size() - 1 ? "" : " ");
		}
		return result;
	}
};