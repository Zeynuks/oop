#include "./core/Calculator.hpp"
#include "CalculatorController.hpp"
#include "Menu.hpp"
#include "ParseArguments.hpp"

#include <exception>
#include <iostream>

Menu InitCalculatorMenu(Calculator& calculator)
{
	Menu menu;

	menu.AddItem("var", "Declare a new variable: var <name> (initial value nan)",
		[&](const auto& args) {
			auto parsedArgs = ParseArguments(args);
			CalculatorController::DeclareVar(calculator, parsedArgs);
		});
	menu.AddItem("let", "Assign value: let <var> = <number|var>",
		[&](const auto& args) {
			auto parsedArgs = ParseArguments(args);
			CalculatorController::AssignVar(calculator, parsedArgs);
		});
	menu.AddItem("fn", "Define a function: fn <name> = <id> | <id><op><id> (+,-,*,/)",
		[&](const auto& args) {
			auto parsedArgs = ParseArguments(args);
			CalculatorController::DefineFunction(calculator, parsedArgs);
		});
	menu.AddItem("print", "Print value: print <id> (2 decimals, nan if undefined)",
		[&](const auto& args) {
			auto parsedArgs = ParseArguments(args);
			CalculatorController::Print(calculator, parsedArgs, std::cout);
		});
	menu.AddItem("printvars", "Print all variables: <name>:<value> sorted alphabetically",
		[&](const auto& args) {
			auto parsedArgs = ParseArguments(args);
			CalculatorController::PrintVars(calculator, std::cout);
		});
	menu.AddItem("printfns", "Print all functions: <name>:<value> sorted alphabetically",
		[&](const auto& args) {
			auto parsedArgs = ParseArguments(args);
			CalculatorController::PrintFunctions(calculator, std::cout);
		});
	menu.AddItem("exit", "Close menu",
		[&menu](const auto& /*args*/) {
			menu.Exit();
		});

	return menu;
}

int main()
{
	try
	{
		Calculator calculator;
		Menu calculatorMenu = InitCalculatorMenu(calculator);

		calculatorMenu.ShowInstructions();
		calculatorMenu.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}