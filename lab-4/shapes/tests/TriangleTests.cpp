#include "../core/Triangle.hpp"
#include "MockCanvas.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <cmath>

using namespace testing;

class TriangleTest : public Test {
protected:
    Point p1{0.0, 0.0};
    Point p2{3.0, 0.0};
    Point p3{0.0, 4.0};
    Color outlineColor{255, 0, 0};
    Color fillColor{0, 255, 0};
};

TEST_F(TriangleTest, GetPerimeter_ReturnsCorrectValue) {
    Triangle triangle(p1, p2, p3, outlineColor, fillColor);
    EXPECT_DOUBLE_EQ(triangle.GetPerimeter(), 12.0);
}

TEST_F(TriangleTest, GetArea_ReturnsCorrectValue) {
    Triangle triangle(p1, p2, p3, outlineColor, fillColor);
    EXPECT_THAT(triangle.GetArea(), DoubleNear(6.0, 1e-9));
}

TEST_F(TriangleTest, GetColors_ReturnConstructorValues) {
    Triangle triangle(p1, p2, p3, outlineColor, fillColor);
    EXPECT_EQ(triangle.GetOutlineColor(), outlineColor);
    EXPECT_EQ(triangle.GetFillColor(), fillColor);
}

TEST_F(TriangleTest, ToString_ReturnsFormattedString) {
    Triangle triangle(p1, p2, p3, outlineColor, fillColor);
    std::string expected = "Triangle:\n\tPoint 1: (0.00, 0.00)\n\tPoint 2: (3.00, 0.00)\n\tPoint 3: (0.00, 4.00)\n";
    EXPECT_EQ(triangle.ToString(), expected);
}

TEST(TriangleBoundary, DegeneratePointsInLine_AreaIsZero) {
    Point a(0.0, 0.0);
    Point b(5.0, 0.0);
    Point c(10.0, 0.0);
    Color black(0, 0, 0);

    Triangle t(a, b, c, black, black);
    EXPECT_THAT(t.GetArea(), DoubleNear(0.0, 1e-9));
    EXPECT_DOUBLE_EQ(t.GetPerimeter(), 20.0);
}

TEST(TriangleBoundary, AllPointsCoincide_AreaAndPerimeterAreZero) {
    Point p(5.0, 5.0);
    Color black(0, 0, 0);

    Triangle t(p, p, p, black, black);
    EXPECT_DOUBLE_EQ(t.GetArea(), 0.0);
    EXPECT_DOUBLE_EQ(t.GetPerimeter(), 0.0);
}

TEST(TriangleBoundary, LargeCoordinates_CalculatesCorrectly) {
    Point a(0.0, 0.0);
    Point b(1000000.0, 0.0);
    Point c(0.0, 1000000.0);
    Color black(0, 0, 0);

    Triangle t(a, b, c, black, black);
    EXPECT_THAT(t.GetArea(), DoubleNear(500000000000.0, 1.0));
}

TEST_F(TriangleTest, Draw_InteractsWithCanvas) {
    std::stringstream output;
    MockCanvas canvas(output);
    Triangle triangle(p1, p2, p3, outlineColor, fillColor);

    triangle.Draw(canvas);
    std::string result = output.str();

    EXPECT_THAT(result, HasSubstr("FillPolygon"));
    EXPECT_THAT(result, HasSubstr("DrawPolygon"));
    EXPECT_THAT(result, HasSubstr("(0.00, 0.00)"));
    EXPECT_THAT(result, HasSubstr("(3.00, 0.00)"));
    EXPECT_THAT(result, HasSubstr("(0.00, 4.00)"));
}