#pragma once

#include "ShapeController.hpp"

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
	explicit ShapeParser(ShapeController& shapeController)
		: m_shapeController(shapeController)
	{
	}

	void Parse(std::istream& input) const
	{
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
				ParseCircle(stream);
			}
			else if (shapeType == ShapeNames::line)
			{
				ParseLine(stream);
			}
			else if (shapeType == ShapeNames::rectangle)
			{
				ParseRectangle(stream);
			}
			else if (shapeType == ShapeNames::triangle)
			{
				ParseTriangle(stream);
			}
			else
			{
				std::cerr << "Error: " << "unknown shape type '" << shapeType << "'" << std::endl;
			}
		}
	}

private:
	ShapeController& m_shapeController;

	static bool CheckStream(const std::istream& input)
	{
		if (input.fail())
		{
			std::cerr << "Error: " << "invalid parameters" << std::endl;
			return false;
		}

		return true;
	}

	void ParseCircle(std::istream& input) const
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

		m_shapeController.AddCircle(radius, center, outlineColor, fillColor);
	}

	void ParseLine(std::istream& input) const
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

		m_shapeController.AddLine(startPoint, endPoint, outlineColor);
	}

	void ParseRectangle(std::istream& input) const
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

		m_shapeController.AddRectangle(startPoint, endPoint, outlineColor, fillColor);
	}

	void ParseTriangle(std::istream& input) const
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

		m_shapeController.AddTriangle(point1, point2, point3, outlineColor, fillColor);
	}
};