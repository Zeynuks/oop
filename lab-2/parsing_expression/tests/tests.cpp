#include <gtest/gtest.h>
#include "../ExpressionParser.h"

TEST(ParseExpressionValid, SingleArgumentPlus)
{
	EXPECT_EQ(ParseExpression("(+ 7)"), 7);
}

TEST(ParseExpressionValid, SingleArgumentMul)
{
	EXPECT_EQ(ParseExpression("(* 8)"), 8);
}

TEST(ParseExpressionValid, TwoArgumentsPlus)
{
	EXPECT_EQ(ParseExpression("(+ 2 3)"), 5);
}

TEST(ParseExpressionValid, ManyArgumentsPlus)
{
	EXPECT_EQ(ParseExpression("(+ 2 3 4)"), 9);
}

TEST(ParseExpressionValid, TwoArgumentsMul)
{
	EXPECT_EQ(ParseExpression("(* 2 4)"), 8);
}

TEST(ParseExpressionValid, ManyArgumentsMul)
{
	EXPECT_EQ(ParseExpression("(* 2 3 4)"), 24);
}

TEST(ParseExpressionValid, NestedExpressionsPlus)
{
	EXPECT_EQ(
		ParseExpression("(+ (* 2 3) (* 3 4))"),
		18
	);
}

TEST(ParseExpressionValid, NestedExpressionsMul)
{
	EXPECT_EQ(
		ParseExpression("(* (+ 1 2) (+ 3 1))"),
		12
	);
}

TEST(ParseExpressionValid, DeepNested)
{
	EXPECT_EQ(
		ParseExpression("(+ 5 (* 2 3 2) (+ 5 (+ 2 5) (* 2 2)))"),
		33
	);
}

TEST(ParseExpressionValid, NegativeNumbers)
{
	EXPECT_EQ(ParseExpression("(+ -2 3)"), 1);
}

TEST(ParseExpressionValid, MultipleNegativeNumbers)
{
	EXPECT_EQ(ParseExpression("(* -2 -3 4)"), 24);
}

TEST(ParseExpressionValid, SpacesAreIgnored)
{
	EXPECT_EQ(
		ParseExpression("(  +   2   (*  3   4 )   )"),
		14
	);
}

TEST(ParseExpressionInvalid, EmptyLine)
{
	EXPECT_THROW(ParseExpression(""), std::runtime_error);
}

TEST(ParseExpressionInvalid, EmptyExpression)
{
	EXPECT_THROW(ParseExpression("()"), std::runtime_error);
}

TEST(ParseExpressionInvalid, MissingArguments)
{
	EXPECT_THROW(ParseExpression("(+)"), std::runtime_error);
}

TEST(ParseExpressionInvalid, UnknownOperation)
{
	EXPECT_THROW(ParseExpression("(% 1 2)"), std::runtime_error);
}

TEST(ParseExpressionInvalid, ExtraCharacters)
{
	EXPECT_THROW(ParseExpression("(+ 1 2) abc"), std::runtime_error);
}
