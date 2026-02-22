#include "../Calculator.hpp"

#include <cmath>
#include <gtest/gtest.h>

class CalculatorTest : public testing::Test
{
protected:
	Calculator calc;
};

TEST_F(CalculatorTest, DeclareVariable)
{
	EXPECT_TRUE(calc.DeclareVariable("v1"));
	EXPECT_TRUE(std::isnan(calc.GetValue("v1")));
	EXPECT_THROW(calc.DeclareVariable("v1"), std::invalid_argument);
}

TEST_F(CalculatorTest, AssignVariable)
{
	calc.DeclareVariable("v1");
	EXPECT_TRUE(calc.AssignVariable("v1", 10.5));
	EXPECT_DOUBLE_EQ(calc.GetValue("v1"), 10.5);

	EXPECT_THROW(calc.AssignVariable("v2", 5.0), std::invalid_argument);

	calc.DefineFunction("f1", "v1");
	EXPECT_THROW(calc.AssignVariable("f1", 5.0), std::invalid_argument);
}

TEST_F(CalculatorTest, AssignVariableFromId)
{
	calc.DeclareVariable("v1");
	calc.DeclareVariable("v2");
	calc.AssignVariable("v1", 20.0);

	EXPECT_TRUE(calc.AssignVariableFromId("v2", "v1"));
	EXPECT_DOUBLE_EQ(calc.GetValue("v2"), 20.0);

	EXPECT_THROW(calc.AssignVariableFromId("v3", "v1"), std::invalid_argument);

	calc.DefineFunction("f1", "v1");
	EXPECT_THROW(calc.AssignVariableFromId("f1", "v1"), std::invalid_argument);
}

TEST_F(CalculatorTest, DefineFunctionUnary)
{
	calc.DeclareVariable("v1");
	calc.AssignVariable("v1", 5.0);

	EXPECT_TRUE(calc.DefineFunction("f1", "v1"));
	EXPECT_DOUBLE_EQ(calc.GetValue("f1"), 5.0);

	EXPECT_THROW(calc.DefineFunction("v1", "v1"), std::invalid_argument);
	EXPECT_THROW(calc.DefineFunction("f2", "missing"), std::invalid_argument);
}

TEST_F(CalculatorTest, DefineFunctionBinary)
{
	calc.DeclareVariable("v1");
	calc.DeclareVariable("v2");
	calc.AssignVariable("v1", 10.0);
	calc.AssignVariable("v2", 2.0);

	EXPECT_TRUE(calc.DefineFunction("add", "v1", Calculator::Operation::Add, "v2"));
	EXPECT_TRUE(calc.DefineFunction("sub", "v1", Calculator::Operation::Sub, "v2"));
	EXPECT_TRUE(calc.DefineFunction("mul", "v1", Calculator::Operation::Mul, "v2"));
	EXPECT_TRUE(calc.DefineFunction("div", "v1", Calculator::Operation::Div, "v2"));

	EXPECT_DOUBLE_EQ(calc.GetValue("add"), 12.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("sub"), 8.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("mul"), 20.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("div"), 5.0);

	EXPECT_THROW(calc.DefineFunction("add", "v1", Calculator::Operation::Add, "v2"), std::invalid_argument);
	EXPECT_THROW(calc.DefineFunction("f_err", "missing", Calculator::Operation::Add, "v2"), std::invalid_argument);
	EXPECT_THROW(calc.DefineFunction("f_err", "v1", Calculator::Operation::Add, "missing"), std::invalid_argument);
}

TEST_F(CalculatorTest, GetValueErrors)
{
	EXPECT_THROW(calc.GetValue("none"), std::invalid_argument);
}

TEST_F(CalculatorTest, GetAll)
{
	calc.DeclareVariable("v1");
	calc.AssignVariable("v1", 1.0);
	calc.DefineFunction("f1", "v1");

	auto vars = calc.GetAllVariables();
	EXPECT_EQ(vars.size(), 1);
	EXPECT_EQ(vars.at("v1"), 1.0);

	auto funcs = calc.GetAllFunctions();
	EXPECT_EQ(funcs.size(), 1);
	EXPECT_EQ(funcs.at("f1"), 1.0);
}

TEST_F(CalculatorTest, CacheAndRecomputation)
{
	calc.DeclareVariable("v1");
	calc.AssignVariable("v1", 2.0);
	calc.DefineFunction("f1", "v1", Calculator::Operation::Mul, "v1");

	EXPECT_DOUBLE_EQ(calc.GetValue("f1"), 4.0);

	calc.AssignVariable("v1", 3.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("f1"), 9.0);
}

TEST_F(CalculatorTest, ComplexDependencyStack)
{
	calc.DeclareVariable("a");
	calc.AssignVariable("a", 2);
	calc.DefineFunction("f1", "a");
	calc.DefineFunction("f2", "f1", Calculator::Operation::Add, "a");
	calc.DefineFunction("f3", "f2", Calculator::Operation::Mul, "f1");

	EXPECT_DOUBLE_EQ(calc.GetValue("f3"), 8.0);
}

TEST_F(CalculatorTest, DivisionByZero)
{
	calc.DeclareVariable("a");
	calc.DeclareVariable("zero");
	calc.AssignVariable("a", 5.0);
	calc.AssignVariable("zero", 0.0);
	calc.DefineFunction("f", "a", Calculator::Operation::Div, "zero");

	EXPECT_THROW(calc.GetValue("f"), std::runtime_error);
}

TEST_F(CalculatorTest, NaNInOperations)
{
	calc.DeclareVariable("nan_v");
	calc.DeclareVariable("a");
	calc.AssignVariable("a", 1.0);
	calc.DefineFunction("f", "a", Calculator::Operation::Add, "nan_v");

	EXPECT_THROW(calc.GetValue("f"), std::runtime_error);
}

TEST_F(CalculatorTest, InternalLogicCoverage)
{
	calc.DeclareVariable("a");
	calc.AssignVariable("a", 1.0);

	struct FakeFunction
	{
		std::string l, r;
		Calculator::Operation op;
		bool b;
	};
	calc.DefineFunction("bad_op", "a", static_cast<Calculator::Operation>(99), "a");
	EXPECT_THROW(calc.GetValue("bad_op"), std::logic_error);
}