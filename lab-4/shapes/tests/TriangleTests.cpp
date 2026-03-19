#include "../core/Triangle.hpp"
#include "MockCanvas.hpp"

#include <cmath>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

using namespace testing;

class TriangleTest : public Test
{
protected:
	Point p1{ 0.0, 0.0 };
	Point p2{ 3.0, 0.0 };
	Point p3{ 0.0, 4.0 };
	Color outlineColor{ 255, 0, 0 };
	Color fillColor{ 0, 255, 0 };
};

TEST_F(TriangleTest, GetPerimeter_ReturnsCorrectValue)
{
	Triangle triangle(p1, p2, p3, outlineColor, fillColor);
	EXPECT_DOUBLE_EQ(triangle.GetPerimeter(), 12.0);
}

TEST_F(TriangleTest, GetArea_ReturnsCorrectValue)
{
	Triangle triangle(p1, p2, p3, outlineColor, fillColor);
	EXPECT_THAT(triangle.GetArea(), DoubleNear(6.0, 1e-9));
}

TEST_F(TriangleTest, GetColors_ReturnConstructorValues)
{
	const Triangle triangle(p1, p2, p3, outlineColor, fillColor);
	EXPECT_EQ(triangle.GetOutlineColor(), outlineColor);
	EXPECT_EQ(triangle.GetFillColor(), fillColor);
}

TEST_F(TriangleTest, ToString_ReturnsFormattedString)
{
	Triangle triangle(p1, p2, p3, outlineColor, fillColor);
	const std::string expected = "Triangle:\n\tPoint 1: (0.00, 0.00)\n\tPoint 2: (3.00, 0.00)\n\tPoint 3: (0.00, 4.00)\n";
	EXPECT_EQ(triangle.ToString(), expected);
}

TEST(TriangleBoundary, DegeneratePointsInLine_AreaIsZero)
{
	constexpr Point a(0.0, 0.0);
	constexpr Point b(5.0, 0.0);
	constexpr Point c(10.0, 0.0);
	constexpr Color black(0, 0, 0);

	Triangle t(a, b, c, black, black);
	EXPECT_THAT(t.GetArea(), DoubleNear(0.0, 1e-9));
	EXPECT_DOUBLE_EQ(t.GetPerimeter(), 20.0);
}

TEST(TriangleBoundary, AllPointsCoincide_AreaAndPerimeterAreZero)
{
	constexpr Point p(5.0, 5.0);
	constexpr Color black(0, 0, 0);

	Triangle t(p, p, p, black, black);
	EXPECT_DOUBLE_EQ(t.GetArea(), 0.0);
	EXPECT_DOUBLE_EQ(t.GetPerimeter(), 0.0);
}

TEST(TriangleBoundary, LargeCoordinates_CalculatesCorrectly)
{
	constexpr Point a(0.0, 0.0);
	constexpr Point b(1000000.0, 0.0);
	constexpr Point c(0.0, 1000000.0);
	constexpr Color black(0, 0, 0);

	Triangle t(a, b, c, black, black);
	EXPECT_THAT(t.GetArea(), DoubleNear(500000000000.0, 1.0));
}

TEST_F(TriangleTest, Draw_InteractsWithCanvas)
{
	MockCanvas canvas;
	const Triangle triangle(p1, p2, p3, outlineColor, fillColor);

	const std::vector<Point> expectedPoints = { p1, p2, p3 };

	EXPECT_CALL(canvas, FillPolygon(expectedPoints, fillColor)).Times(1);
	EXPECT_CALL(canvas, DrawPolygon(expectedPoints, outlineColor)).Times(1);

	triangle.Draw(canvas);
}