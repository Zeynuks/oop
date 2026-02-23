#pragma once

#include "commands/ICommand.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

class Menu
{
public:
	void AddItem(const std::string& shortcut, const std::string& description, std::unique_ptr<ICommand>&& command);
	void Run();
	void ShowInstructions() const;
	void Exit();

private:
	struct Item
	{
		Item(std::string shortcut, std::string description, std::unique_ptr<ICommand>&& command);

		std::string shortcut;
		std::string description;
		std::unique_ptr<ICommand> command;
	};

	std::vector<Item> m_items;
	bool m_exit = false;

	static std::pair<std::string, std::vector<std::string>> ParseLine(const std::string& line);
	bool ExecuteCommand(const std::string& line);
};