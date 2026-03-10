#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

class Menu
{
public:
	using Action = std::function<void(const std::vector<std::string>& args)>;

	void AddItem(std::string shortcut, std::string description, Action action)
	{
		m_items.push_back({ std::move(shortcut), std::move(description), std::move(action) });
	}

	void ShowInstructions() const
	{
		std::cout << "Commands list:" << std::endl;
		for (const auto& item : m_items)
		{
			std::cout << "  " << item.shortcut << ": " << item.description << std::endl;
		}
	}

	void Exit()
	{
		m_exit = true;
	}

	void Run()
	{
		m_exit = false;
		std::string line;

		while (!m_exit && (std::cout << "> ", std::getline(std::cin, line)))
		{
			auto [shortcut, args] = ParseLine(line);

			if (shortcut.empty())
			{
				continue;
			}

			auto it = std::ranges::find_if(m_items, [&](const Item& item) {
				return item.shortcut == shortcut;
			});

			if (it != m_items.end())
			{
				try
				{
					it->action(args);
				}
				catch (const std::exception& e)
				{
					std::cout << "Error: " << e.what() << std::endl;
				}
			}
			else
			{
				std::cout << "Unknown command. Type 'Help' for instructions." << std::endl;
			}
		}
	}

private:
	struct Item
	{
		std::string shortcut;
		std::string description;
		Action action;
	};

	static std::pair<std::string, std::vector<std::string>> ParseLine(std::string_view line)
	{
		auto words = line
			| std::views::split(' ')
			| std::views::filter([](auto&& rng) { return !rng.empty(); })
			| std::views::transform([](auto&& rng) {
				  return std::string(rng.begin(), rng.end());
			  })
			| std::ranges::to<std::vector<std::string>>();

		if (words.empty())
		{
			return { "", {} };
		}

		std::string shortcut = words[0];
		std::vector args(words.begin() + 1, words.end());

		return { shortcut, args };
	}

	std::vector<Item> m_items;
	bool m_exit = false;
};