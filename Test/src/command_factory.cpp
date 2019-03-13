//
// Created by yanik on 08/03/19.
//

#include "command_factory.h"

#include "mute_bgm_command.h"
#include "menu_commands.h"
#include "command_exit_game.h"

std::shared_ptr<Command> CommandFactory::create_mute_bgm_command(std::string* text)
{
    return std::shared_ptr<Command>(new MuteBGMCommand(text));
}

std::shared_ptr<Command> CommandFactory::create_exit_game_command()
{
    return std::shared_ptr<Command>(new ExitGameCommand());
}

std::shared_ptr<Command> CommandFactory::create_exit_menu_command(MenuSettings* ms){
    auto result = std::shared_ptr<Command>(new ExitMenuCommand(ms));
    return result;
}
