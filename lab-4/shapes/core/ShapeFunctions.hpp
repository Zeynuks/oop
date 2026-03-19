#pragma once

#include "IShape.hpp"

#include <algorithm>
#include <ranges>
#include <stdexcept>

namespace ShapeFunctions
{
IShape& FindMaxAreaShape(const std::ranges::forward_range auto& shapes)
{
	if (std::ranges::begin(shapes) == std::ranges::end(shapes))
	{
		throw std::runtime_error("No shapes available");
	}

	const auto it = std::ranges::max_element(shapes,
		[](const auto& a, const auto& b) {
			return a->GetArea() < b->GetArea();
		});

	return **it;
}

IShape& FindMinPerimeterShape(const std::ranges::forward_range auto& shapes)
{
	if (std::ranges::begin(shapes) == std::ranges::end(shapes))
	{
		throw std::runtime_error("No shapes available");
	}

	const auto it = std::ranges::min_element(shapes,
		[](const auto& a, const auto& b) {
			return a->GetPerimeter() < b->GetPerimeter();
		});

	return **it;
}
}