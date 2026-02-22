#include "Car.hpp"
#include "Menu.hpp"
#include "commands/CarInfoCommand.hpp"
#include "commands/SetGearCommand.h"
#include "commands/SetSpeedCommand.hpp"
#include "commands/TurnOffEngineCommand.hpp"
#include "commands/TurnOnCommand.hpp"

#include <exception>
#include <iostream>

Menu InitCarMenu(Car& car)
{
	Menu menu;

	menu.AddItem("Info", "Show current car status",
		std::make_unique<CarInfoCommand>(car));
	menu.AddItem("EngineOn", "Turn the car engine on",
		std::make_unique<TurnOnEngineCommand>(car));
	menu.AddItem("EngineOff", "Turn the car engine off",
		std::make_unique<TurnOffEngineCommand>(car));
	menu.AddItem("SetGear", "Set the car gear: "
							"\n\t\t\tReverse: -1"
							"\n\t\t\tNeutral: 0"
							"\n\t\t\tForward gears: 1-5",
		std::make_unique<SetGearCommand>(car));
	menu.AddItem("SetSpeed", "Set the car speed",
		std::make_unique<SetSpeedCommand>(car));

	return menu;
}

int main()
{
	try
	{
		Car car;
		Menu carMenu = InitCarMenu(car);

		carMenu.ShowInstructions();
		carMenu.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}