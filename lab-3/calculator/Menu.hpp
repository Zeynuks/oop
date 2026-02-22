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
	void AddItem(const std::string& shortcut, const std::string& description,
		std::unique_ptr<ICommand>&& command)
	{
		m_items.emplace_back(shortcut, description, std::move(command));
	}
	void Run()
	{
		std::string command;
		while (std::cout << "> " && getline(std::cin, command) && ExecuteCommand(command))
		{
		}
	}
	void ShowInstructions() const
	{
		std::cout << "Commands list:" << std::endl;
		for (auto& item : m_items)
		{
			std::cout << "  " << item.shortcut << ": " << item.description << std::endl;
		}
	}
	void Exit()
	{
		m_exit = true;
	}

private:
	struct Item
	{
		Item(std::string shortcut, std::string description,
			std::unique_ptr<ICommand>&& command)
			: shortcut(std::move(shortcut))
			, description(std::move(description))
			, command(std::move(command))
		{
		}
		std::string shortcut;
		std::string description;
		std::unique_ptr<ICommand> command;
	};

	std::vector<Item> m_items;
	bool m_exit = false;

	static std::pair<std::string, std::vector<std::string>> ParseLine(const std::string& line)
	{
		std::istringstream iss(line);
		std::string shortcut;
		iss >> shortcut;

		std::vector<std::string> args;
		std::string arg;
		while (iss >> arg)
			args.push_back(arg);

		return { shortcut, args };
	}

	bool ExecuteCommand(const std::string& line)
	{
		m_exit = false;

		auto [shortcut, args] = ParseLine(line);

		const auto it = std::ranges::find_if(m_items, [&](const Item& item) {
			return item.shortcut == shortcut;
		});

		if (it != m_items.end())
		{
			try
			{
				it->command->ParseArgs(args);
				it->command->Execute();
			}
			catch (const std::exception& e)
			{
				std::cout << "Error: " << e.what() << std::endl;
			}
		}
		else
		{
			std::cout << "Unknown command" << std::endl;
		}

		return !m_exit;
	}
};