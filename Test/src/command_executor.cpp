//
// Created by yanik on 08/03/19.
//

#include "command_executor.h"

CommandExecutor::CommandExecutor(){
    m_command = nullptr;
}

void CommandExecutor::execute_command()
{
    if(m_command)
        m_command->execute();
}

void CommandExecutor::store_command(std::shared_ptr <Command> command)
{
    m_command = command;
}
