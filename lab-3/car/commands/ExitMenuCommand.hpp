#pragma once

#include "../Menu.hpp"
#include "ICommand.hpp"

class CExitMenuCommand : public ICommand
{
public:
	explicit CExitMenuCommand(Menu& menu)
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