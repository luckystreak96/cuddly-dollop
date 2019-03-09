//
// Created by yanik on 08/03/19.
//

#include "command_exit_game.h"

#include "glfwBackend.h"

void ExitGameCommand::execute()
{
    GLFWManager::Exit();
}