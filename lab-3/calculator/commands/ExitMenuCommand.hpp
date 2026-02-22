#pragma once

#include "../Menu.hpp"
#include "ICommand.hpp"

class ExitMenuCommand : public ICommand
{
public:
	explicit ExitMenuCommand(Menu& menu)
		: m_menu(menu)
	{
	}

	void Execute() override
	{
		m_menu.Exit();
	}

private:
	Menu& m_menu;
};