#include "../core/LineSegment.hpp"
#include "MockCanvas.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>

using namespace testing;

class LineSegmentTest : public Test {
protected:
    Point p1{0.0, 0.0};
    Point p2{3.0, 4.0};
    Color outlineColor{255, 255, 255};
};

TEST_F(LineSegmentTest, GetArea_AlwaysReturnsZero) {
    LineSegment line(p1, p2, outlineColor);
    EXPECT_DOUBLE_EQ(line.GetArea(), 0.0);
}

TEST_F(LineSegmentTest, GetPerimeter_ReturnsDistanceBetweenPoints) {
    LineSegment line(p1, p2, outlineColor);
    EXPECT_DOUBLE_EQ(line.GetPerimeter(), 5.0);
}

TEST_F(LineSegmentTest, GetPoints_ReturnConstructorValues) {
	const LineSegment line(p1, p2, outlineColor);
    EXPECT_EQ(line.GetStartPoint().x, p1.x);
    EXPECT_EQ(line.GetStartPoint().y, p1.y);
    EXPECT_EQ(line.GetEndPoint().x, p2.x);
    EXPECT_EQ(line.GetEndPoint().y, p2.y);
}

TEST_F(LineSegmentTest, GetOutlineColor_ReturnsConstructorValue) {
	const LineSegment line(p1, p2, outlineColor);
    EXPECT_EQ(line.GetOutlineColor(), outlineColor);
}

TEST_F(LineSegmentTest, ToString_ReturnsFormattedString) {
    LineSegment line(p1, p2, outlineColor);
	const std::string result = line.ToString();
    EXPECT_THAT(result, HasSubstr("LineSegment:"));
    EXPECT_THAT(result, HasSubstr("StartPoint: (0.00, 0.00)"));
    EXPECT_THAT(result, HasSubstr("EndPoint: (3.00, 4.00)"));
    EXPECT_THAT(result, HasSubstr("Length: 5.00"));
}

TEST(LineSegmentBoundary, ZeroLength_PointsCoincide) {
	constexpr Point p{1.0, 1.0};
	constexpr Color black{0, 0, 0};
    LineSegment line(p, p, black);

    EXPECT_DOUBLE_EQ(line.GetPerimeter(), 0.0);
}

TEST_F(LineSegmentTest, Draw_InteractsWithCanvas) {
    std::stringstream output;
    MockCanvas canvas(output);
	const LineSegment line(p1, p2, outlineColor);

    line.Draw(canvas);
	const std::string result = output.str();

    EXPECT_THAT(result, HasSubstr("DrawLine"));
    EXPECT_THAT(result, HasSubstr("from (0.00, 0.00)"));
    EXPECT_THAT(result, HasSubstr("to (3.00, 4.00)"));
}