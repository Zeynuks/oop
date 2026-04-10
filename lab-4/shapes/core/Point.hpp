#pragma once

#include <cmath>
#include <format>
#include <string>

struct Point
{
	double x = 0;
	double y = 0;

	bool operator==(const Point&) const = default;

	double DistanceTo(const Point other) const
	{
		return std::hypot(x - other.x, y - other.y);
	}
};

template <>
struct std::formatter<Point> : std::formatter<std::string_view> {
	static auto format(const Point& p, std::format_context& ctx)
	{
		return std::format_to(ctx.out(), "({:.2f}, {:.2f})", p.x, p.y);
	}
};