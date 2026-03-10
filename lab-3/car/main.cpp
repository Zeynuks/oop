#include "Car.hpp"
#include "CarController.hpp"
#include "Menu.hpp"

#include <exception>
#include <iostream>

Menu InitCarMenu(Car& car)
{
	Menu menu;

	menu.AddItem("Info", "Show current car status",
		[&car](const auto& args) {
			CarController::ShowInfo(car);
		});
	menu.AddItem("EngineOn", "Turn the car engine on",
		[&car](const auto& args) {
			CarController::TurnOnEngine(car);
		});
	menu.AddItem("EngineOff", "Turn the car engine off",
		[&car](const auto& args) {
			CarController::TurnOffEngine(car);
		});
	menu.AddItem("SetGear", "Set the car gear: "
					  "\n\t\t\tReverse: -1"
					  "\n\t\t\tNeutral: 0"
					  "\n\t\t\tForward gears: 1-5",
		[&car](const auto& args) {
			CarController::SetGear(car, args);
		});
	menu.AddItem("SetSpeed", "Set the car speed",
		[&car](const auto& args) {
			CarController::SetSpeed(car, args);
		});
	menu.AddItem("Help", "Show this help message",
		[&menu](const auto& args) {
			menu.ShowInstructions();
		});
	menu.AddItem("Exit", "Exit from menu",
		[&menu](const auto& args) {
			menu.Exit();
		});

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