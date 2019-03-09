//
// Created by yanik on 08/03/19.
//

#ifndef CUDDLY_DOLLOP_COMMAND_FACTORY_H
#define CUDDLY_DOLLOP_COMMAND_FACTORY_H

#include <memory>
#include "command.h"
#include "menuSettings.h"

class CommandFactory
{
public:
    static std::shared_ptr<Command> create_mute_bgm_command();
    static std::shared_ptr<Command> create_exit_menu_command(MenuSettings* ms);
    static std::shared_ptr<Command> create_exit_game_command();
};

#endif //CUDDLY_DOLLOP_COMMAND_FACTORY_H
