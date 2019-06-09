//
// Created by yanik on 08/03/19.
//

#ifndef CUDDLY_DOLLOP_SETTINGS_COMMANDS_H
#define CUDDLY_DOLLOP_SETTINGS_COMMANDS_H

#include "command.h"
#include "menuSettings.h"

class MenuCommand : public Command
{
public:
    MenuCommand(MenuSettings* ms);
    virtual void execute() {}

protected:
    MenuSettings* _menu_settings;
};


class ExitMenuCommand : public MenuCommand
{
public:
    using MenuCommand::MenuCommand;
    void execute();
};

#endif //CUDDLY_DOLLOP_SETTINGS_COMMANDS_H
