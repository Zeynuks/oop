#include "Menu.hpp"
#include "./core/Calculator.hpp"
#include "commands/AssignVariableCommand.hpp"
#include "commands/DeclareVariableCommand.hpp"
#include "commands/DefineFunctionCommand.hpp"
#include "commands/ExitMenuCommand.hpp"
#include "commands/PrintCommand.hpp"
#include "commands/PrintFunctionCommand.hpp"
#include "commands/PrintVariablesCommand.hpp"

#include <exception>
#include <iostream>

Menu InitCalculatorMenu(Calculator& calculator)
{
	Menu menu;

	menu.AddItem("var", "Declare a new variable: var <name> (initial value nan)",
		std::make_unique<DeclareVariableCommand>(calculator));
	menu.AddItem("let", "Assign value: let <var> = <number|var>",
		std::make_unique<AssignVariableCommand>(calculator));
	menu.AddItem("fn", "Define a function: fn <name> = <id> | <id><op><id> (+,-,*,/)",
		std::make_unique<DefineFunctionCommand>(calculator));
	menu.AddItem("print", "Print value: print <id> (2 decimals, nan if undefined)",
		std::make_unique<PrintCommand>(calculator, std::cout));
	menu.AddItem("printvars", "Print all variables: <name>:<value> sorted alphabetically",
		std::make_unique<PrintVariablesCommand>(calculator, std::cout));
	menu.AddItem("printfns", "Print all functions: <name>:<value> sorted alphabetically",
		std::make_unique<PrintFunctionCommand>(calculator, std::cout));
	menu.AddItem("exit", "Close menu",
		std::make_unique<ExitMenuCommand>(menu));

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