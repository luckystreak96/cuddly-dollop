//
// Created by yanik on 08/03/19.
//

#ifndef CUDDLY_DOLLOP_COMMAND_EXIT_GAME_H
#define CUDDLY_DOLLOP_COMMAND_EXIT_GAME_H

#include "command.h"

class ExitGameCommand : public Command
{
public:
    ExitGameCommand() {}
    void execute();
};

#endif //CUDDLY_DOLLOP_COMMAND_EXIT_GAME_H
