//
// Created by yanik on 08/03/19.
//

#ifndef CUDDLY_DOLLOP_COMMAND_EXECUTOR_H
#define CUDDLY_DOLLOP_COMMAND_EXECUTOR_H

#include "command.h"
#include <memory>

class CommandExecutor
{
public:
    CommandExecutor();
    void store_command(std::shared_ptr<Command> command);
    void execute_command();
protected:
    std::shared_ptr<Command> m_command;
};

#endif //CUDDLY_DOLLOP_COMMAND_EXECUTOR_H
