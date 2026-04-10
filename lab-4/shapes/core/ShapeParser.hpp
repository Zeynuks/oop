#pragma once

#include "Circle.hpp"
#include "LineSegment.hpp"
#include "Rectangle.hpp"
#include "Triangle.hpp"

#include <iostream>
#include <memory>
#include <sstream>

namespace ShapeNames
{
constexpr auto circle = "circle";
constexpr auto line = "line";
constexpr auto rectangle = "rectangle";
constexpr auto triangle = "triangle";
} // namespace ShapeNames

class ShapeParser
{
public:
	explicit ShapeParser()
	{
	}

	static std::vector<std::unique_ptr<IShape>> Parse(std::istream& input)
	{
		std::vector<std::unique_ptr<IShape>> shapes;
		std::string line;

		while (std::getline(input, line))
		{
			if (line.empty())
			{
				continue;
			}

			std::stringstream stream(line);
			std::string shapeType;
			stream >> shapeType;

			if (shapeType == ShapeNames::circle)
			{
				ParseCircle(stream, shapes);
			}
			else if (shapeType == ShapeNames::line)
			{
				ParseLine(stream, shapes);
			}
			else if (shapeType == ShapeNames::rectangle)
			{
				ParseRectangle(stream, shapes);
			}
			else if (shapeType == ShapeNames::triangle)
			{
				ParseTriangle(stream, shapes);
			}
			else
			{
				std::cerr << "Error: " << "unknown shape type '" << shapeType << "'" << std::endl;
			}
		}

		return shapes;
	}

private:

	static bool CheckStream(const std::istream& input)
	{
		if (input.fail())
		{
			std::cerr << "Error: " << "invalid parameters" << std::endl;
			return false;
		}

		return true;
	}

	static void ParseCircle(std::istream& input, std::vector<std::unique_ptr<IShape>>& shapes)
	{
		Point center;
		double radius;
		Color outlineColor = DefaultColors::Transparent;
		Color fillColor = DefaultColors::Green;

		if (!(input >> center.x >> center.y >> radius))
		{
			CheckStream(input);
			return;
		}

		uint32_t colorValue;
		if (input >> std::hex >> colorValue)
		{
			outlineColor = colorValue;
		}

		if (input >> std::hex >> colorValue)
		{
			fillColor = colorValue;
		}

		auto circle = std::make_unique<Circle>(radius, center, outlineColor, fillColor);
		shapes.push_back(std::move(circle));
	}

	static void ParseLine(std::istream& input, std::vector<std::unique_ptr<IShape>>& shapes)
	{
		Point startPoint;
		Point endPoint;
		Color outlineColor;

		if (!(input >> startPoint.x >> startPoint.y >> endPoint.x >> endPoint.y))
		{
			CheckStream(input);
			return;
		}

		if (uint32_t colorValue; input >> std::hex >> colorValue)
		{
			outlineColor = colorValue;
		}

		auto line = std::make_unique<LineSegment>(startPoint, endPoint, outlineColor);
		shapes.push_back(std::move(line));
	}

	static void ParseRectangle(std::istream& input, std::vector<std::unique_ptr<IShape>>& shapes)
	{
		Point startPoint;
		Point endPoint;
		Color outlineColor = DefaultColors::Transparent;
		Color fillColor = DefaultColors::Green;

		if (!(input >> startPoint.x >> startPoint.y >> endPoint.x >> endPoint.y))
		{
			CheckStream(input);
			return;
		}

		uint32_t colorValue;
		if (input >> std::hex >> colorValue)
		{
			outlineColor = colorValue;
		}

		if (input >> std::hex >> colorValue)
		{
			fillColor = colorValue;
		}

		auto rectangle = std::make_unique<Rectangle>(startPoint, endPoint, outlineColor, fillColor);
		shapes.push_back(std::move(rectangle));
	}

	static void ParseTriangle(std::istream& input, std::vector<std::unique_ptr<IShape>>& shapes)
	{
		Point point1;
		Point point2;
		Point point3;
		Color outlineColor = DefaultColors::Transparent;
		Color fillColor = DefaultColors::Green;

		if (!(input >> point1.x >> point1.y >> point2.x >> point2.y >> point3.x >> point3.y))
		{
			CheckStream(input);
			return;
		}

		uint32_t colorValue;
		if (input >> std::hex >> colorValue)
		{
			outlineColor = colorValue;
		}

		if (input >> std::hex >> colorValue)
		{
			fillColor = colorValue;
		}

		auto circle = std::make_unique<Triangle>(point1, point2, point3, outlineColor, fillColor);
		shapes.push_back(std::move(circle));
	}
};