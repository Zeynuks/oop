#include "../core/Rectangle.hpp"
#include "MockCanvas.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>

using namespace testing;

class RectangleTest : public Test
{
protected:
	Point pStart{ 0.0, 0.0 };
	Point pEnd{ 4.0, 3.0 };
	Color outlineColor{ 255, 0, 0 };
	Color fillColor{ 0, 255, 0 };
};

TEST_F(RectangleTest, GetArea_ReturnsCorrectValue)
{
	Rectangle rect(pStart, pEnd, outlineColor, fillColor);
	EXPECT_DOUBLE_EQ(rect.GetArea(), 12.0);
}

TEST_F(RectangleTest, GetPerimeter_ReturnsCorrectValue)
{
	Rectangle rect(pStart, pEnd, outlineColor, fillColor);
	EXPECT_DOUBLE_EQ(rect.GetPerimeter(), 14.0);
}

TEST_F(RectangleTest, GetDimensions_ReturnsCorrectValues)
{
	const Rectangle rect(pStart, pEnd, outlineColor, fillColor);
	EXPECT_DOUBLE_EQ(rect.GetWidth(), 4.0);
	EXPECT_DOUBLE_EQ(rect.GetHeight(), 3.0);
}

TEST_F(RectangleTest, GetColors_ReturnConstructorValues)
{
	const Rectangle rect(pStart, pEnd, outlineColor, fillColor);
	EXPECT_EQ(rect.GetOutlineColor(), outlineColor);
	EXPECT_EQ(rect.GetFillColor(), fillColor);
}

TEST_F(RectangleTest, ToString_ReturnsFormattedString)
{
	Rectangle rect(pStart, pEnd, outlineColor, fillColor);
	const std::string result = rect.ToString();
	EXPECT_THAT(result, HasSubstr("Rectangle:"));
	EXPECT_THAT(result, HasSubstr("Width: 4.00"));
	EXPECT_THAT(result, HasSubstr("Height: 3.00"));
}

TEST(RectangleBoundary, ZeroArea_WhenPointsCoincide)
{
	constexpr Point p(10.0, 10.0);
	constexpr Color black(0, 0, 0);
	Rectangle rect(p, p, black, black);

	EXPECT_DOUBLE_EQ(rect.GetArea(), 0.0);
	EXPECT_DOUBLE_EQ(rect.GetPerimeter(), 0.0);
}

TEST(RectangleBoundary, VerticalLine_AreaIsZero)
{
	constexpr Point p1(0.0, 0.0);
	constexpr Point p2(0.0, 10.0);
	constexpr Color black(0, 0, 0);
	Rectangle rect(p1, p2, black, black);

	EXPECT_DOUBLE_EQ(rect.GetWidth(), 0.0);
	EXPECT_DOUBLE_EQ(rect.GetArea(), 0.0);
	EXPECT_DOUBLE_EQ(rect.GetPerimeter(), 20.0);
}

TEST(RectangleBoundary, NegativeCoordinates_CalculatesCorrectDimensions)
{
	constexpr Point p1(-2.0, -2.0);
	constexpr Point p2(2.0, 2.0);
	constexpr Color black(0, 0, 0);
	Rectangle rect(p1, p2, black, black);

	EXPECT_DOUBLE_EQ(rect.GetWidth(), 4.0);
	EXPECT_DOUBLE_EQ(rect.GetHeight(), 4.0);
	EXPECT_DOUBLE_EQ(rect.GetArea(), 16.0);
}

TEST_F(RectangleTest, Draw_InteractsWithCanvas)
{
	MockCanvas canvas;
	const Rectangle rect(pStart, pEnd, outlineColor, fillColor);

	const std::vector expectedPoints = {
		Point(0.0, 0.0),
		Point(4.0, 0.0),
		Point(4.0, 3.0),
		Point(0.0, 3.0)
	};

	EXPECT_CALL(canvas, FillPolygon(expectedPoints, fillColor)).Times(1);
	EXPECT_CALL(canvas, DrawPolygon(expectedPoints, outlineColor)).Times(1);

	rect.Draw(canvas);
}