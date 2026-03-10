#include "../core/Calculator.hpp"
#include <cmath>
#include <gtest/gtest.h>

class CalculatorTest : public testing::Test
{
protected:
	Calculator calc;
};

TEST_F(CalculatorTest, DeclareVariable)
{
	calc.DeclareVariable("v1");
	EXPECT_TRUE(std::isnan(calc.GetValue("v1")));
	EXPECT_THROW(calc.DeclareVariable("v1"), std::invalid_argument);
}

TEST_F(CalculatorTest, AssignVariable)
{
	calc.AssignVariable("v1", 10.5);
	EXPECT_DOUBLE_EQ(calc.GetValue("v1"), 10.5);

	calc.DefineFunction("f1", "v1");
	EXPECT_THROW(calc.AssignVariable("f1", 5.0), std::invalid_argument);
}

TEST_F(CalculatorTest, AssignVariableFromId)
{
	calc.DeclareVariable("v1");
	calc.AssignVariable("v1", 20.0);

	calc.AssignVariable("v2", "v1");
	EXPECT_DOUBLE_EQ(calc.GetValue("v2"), 20.0);
}

TEST_F(CalculatorTest, DefineFunctionUnary)
{
	calc.DeclareVariable("v1");
	calc.AssignVariable("v1", 5.0);

	calc.DefineFunction("f1", "v1");
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

	calc.DefineFunction("add", { "v1", "v2", Operation::Add });
	calc.DefineFunction("sub", { "v1", "v2", Operation::Sub });
	calc.DefineFunction("mul", { "v1", "v2", Operation::Mul });
	calc.DefineFunction("div", { "v1", "v2", Operation::Div });

	EXPECT_DOUBLE_EQ(calc.GetValue("add"), 12.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("sub"), 8.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("mul"), 20.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("div"), 5.0);

	EXPECT_THROW(calc.DefineFunction("add", { "v1", "v2", Operation::Add }), std::invalid_argument);
	EXPECT_THROW(calc.DefineFunction("bad1", { "missing", "v2", Operation::Add }), std::invalid_argument);
	EXPECT_THROW(calc.DefineFunction("bad2", { "v1", "missing", Operation::Add }), std::invalid_argument);
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

	const auto vars = calc.GetAllVariables();
	EXPECT_EQ(vars.size(), 1);
	EXPECT_EQ(vars.at("v1"), 1.0);

	const auto& funcs = calc.GetAllFunctions();
	EXPECT_EQ(funcs.size(), 1);
	EXPECT_EQ(funcs.at("f1"), 1.0);
}

TEST_F(CalculatorTest, CacheAndRecomputation)
{
	calc.DeclareVariable("v1");
	calc.AssignVariable("v1", 2.0);
	calc.DefineFunction("f1", { "v1", "v1", Operation::Mul });

	EXPECT_DOUBLE_EQ(calc.GetValue("f1"), 4.0);

	calc.AssignVariable("v1", 3.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("f1"), 9.0);
}

TEST_F(CalculatorTest, ComplexDependencyStack)
{
	calc.DeclareVariable("a");
	calc.AssignVariable("a", 2.0);

	calc.DefineFunction("f1", "a");
	calc.DefineFunction("f2", { "f1", "a", Operation::Add });
	calc.DefineFunction("f3", { "f2", "f1", Operation::Mul });

	EXPECT_DOUBLE_EQ(calc.GetValue("f3"), 8.0);
}

TEST_F(CalculatorTest, DivisionByZero)
{
	calc.DeclareVariable("a");
	calc.DeclareVariable("zero");
	calc.AssignVariable("a", 5.0);
	calc.AssignVariable("zero", 0.0);

	calc.DefineFunction("f", { "a", "zero", Operation::Div });
	EXPECT_THROW(calc.GetValue("f"), std::runtime_error);
}

TEST_F(CalculatorTest, NaNInOperations)
{
	calc.DeclareVariable("nan_v");
	calc.DeclareVariable("a");
	calc.AssignVariable("a", 1.0);

	calc.DefineFunction("f", { "a", "nan_v", Operation::Add });

	EXPECT_TRUE(std::isnan(calc.GetValue("f")));
}

TEST_F(CalculatorTest, InvalidOperation)
{
	calc.DeclareVariable("a");
	calc.AssignVariable("a", 1.0);

	EXPECT_THROW(
		calc.DefineFunction("bad_op", { "a", "a", static_cast<Operation>(99) }),
		std::logic_error);
}

TEST_F(CalculatorTest, DeepLinearChain_NoStackOverflow)
{
	calc.DeclareVariable("x");
	calc.AssignVariable("x", 1.0);

	calc.DefineFunction("x1", { "x", "x", Operation::Add });

	constexpr int depth = 100000;
	for (int i = 2; i <= depth; ++i)
	{
		calc.DefineFunction(
			"x" + std::to_string(i),
			{ "x" + std::to_string(i - 1), "x", Operation::Add });
	}

	EXPECT_DOUBLE_EQ(calc.GetValue("x100000"), 100001.0);

	calc.AssignVariable("x", 2.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("x100000"), 200002.0);
}

TEST_F(CalculatorTest, FibonacciOptimizedComputation)
{
	calc.DeclareVariable("v0");
	calc.DeclareVariable("v1");
	calc.AssignVariable("v0", 0.0);
	calc.AssignVariable("v1", 1.0);

	calc.DefineFunction("fib0", "v0");
	calc.DefineFunction("fib1", "v1");

	for (int i = 2; i <= 50; ++i)
	{
		calc.DefineFunction(
			"fib" + std::to_string(i),
			{ "fib" + std::to_string(i - 1),
				"fib" + std::to_string(i - 2),
				Operation::Add });
	}

	EXPECT_DOUBLE_EQ(calc.GetValue("fib50"), 12586269025.0);

	calc.AssignVariable("v0", 1.0);
	EXPECT_DOUBLE_EQ(calc.GetValue("fib50"), 20365011074.0);
}