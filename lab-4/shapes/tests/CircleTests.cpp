#include "../core/Circle.hpp"
#include "MockCanvas.hpp"

#include <cmath>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

class CircleTest : public Test
{
protected:
	Point center{ 5.0, 5.0 };
	double radius = 10.0;
	Color outlineColor{ 255, 0, 0 };
	Color fillColor{ 0, 255, 0 };
};

TEST_F(CircleTest, GetArea_ReturnsCorrectValue)
{
	Circle circle(radius, center, outlineColor, fillColor);
	EXPECT_DOUBLE_EQ(circle.GetArea(), M_PI * 100.0);
}

TEST_F(CircleTest, GetPerimeter_ReturnsCorrectValue)
{
	Circle circle(radius, center, outlineColor, fillColor);
	EXPECT_DOUBLE_EQ(circle.GetPerimeter(), 2 * M_PI * 10.0);
}

TEST_F(CircleTest, GetParameters_ReturnConstructorValues)
{
	const Circle circle(radius, center, outlineColor, fillColor);
	EXPECT_DOUBLE_EQ(circle.GetRadius(), radius);
	EXPECT_EQ(circle.GetCenter().x, center.x);
	EXPECT_EQ(circle.GetCenter().y, center.y);
}

TEST_F(CircleTest, GetColors_ReturnConstructorValues)
{
	const Circle circle(radius, center, outlineColor, fillColor);
	EXPECT_EQ(circle.GetOutlineColor(), outlineColor);
	EXPECT_EQ(circle.GetFillColor(), fillColor);
}

TEST_F(CircleTest, ToString_ReturnsFormattedString)
{
	Circle circle(radius, center, outlineColor, fillColor);
	const std::string result = circle.ToString();
	EXPECT_THAT(result, HasSubstr("Circle:"));
	EXPECT_THAT(result, HasSubstr("Radius: 10.00"));
	EXPECT_THAT(result, HasSubstr("(5.00, 5.00)"));
}

TEST(CircleBoundary, ZeroRadius_AreaAndPerimeterAreZero)
{
	constexpr Point p(0.0, 0.0);
	constexpr Color black{ 0, 0, 0 };
	Circle circle(0.0, p, black, black);

	EXPECT_DOUBLE_EQ(circle.GetArea(), 0.0);
	EXPECT_DOUBLE_EQ(circle.GetPerimeter(), 0.0);
}

TEST(CircleBoundary, LargeRadius_CalculatesCorrectly)
{
	constexpr Point p(0.0, 0.0);
	constexpr double largeRadius = 1e6;
	constexpr Color black{ 0, 0, 0 };
	Circle circle(largeRadius, p, black, black);

	EXPECT_DOUBLE_EQ(circle.GetArea(), M_PI * 1e12);
}

TEST_F(CircleTest, Draw_InteractsWithCanvas)
{
	MockCanvas canvas;
	const Circle circle(radius, center, outlineColor, fillColor);

	EXPECT_CALL(canvas, FillCircle(center, radius, fillColor)).Times(1);
	EXPECT_CALL(canvas, DrawCircle(center, radius, outlineColor)).Times(1);

	circle.Draw(canvas);
}