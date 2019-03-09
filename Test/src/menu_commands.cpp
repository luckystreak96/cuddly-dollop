//
// Created by yanik on 08/03/19.
//

#include "menu_commands.h"


MenuCommand::MenuCommand(MenuSettings *ms)
{
    _menu_settings = ms;
}

void ExitMenuCommand::execute()
{
    _menu_settings->_done = true;
}