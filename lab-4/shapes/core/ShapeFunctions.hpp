#pragma once

#include "IShape.hpp"

#include <algorithm>
#include <memory>

namespace ShapeFunctions
{
inline IShape& FindMaxAreaShape(std::vector<std::unique_ptr<IShape>>& shapes)
{
	if (shapes.empty())
	{
		throw std::runtime_error("No shapes available");
	}

	const auto it = std::ranges::max_element(shapes,
		[](const std::unique_ptr<IShape>& a, const std::unique_ptr<IShape>& b) {
			return a->GetArea() < b->GetArea();
		});

	return **it;
}

inline IShape& FindMinPerimeterShape(std::vector<std::unique_ptr<IShape>>& shapes)
{
	if (shapes.empty())
	{
		throw std::runtime_error("No shapes available");
	}

	const auto it = std::ranges::min_element(shapes,
		[](const std::unique_ptr<IShape>& a, const std::unique_ptr<IShape>& b) {
			return a->GetPerimeter() < b->GetPerimeter();
		});

	return **it;
}
}