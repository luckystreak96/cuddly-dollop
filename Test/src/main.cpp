#include <iostream>
#include "glutBackend.h"
#include "game.h"
#include "define.h"

int main(int argc, char** argv)
{
    GLUTBackendInit(argc, argv, true, false);

    if(!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, IS_FULLSCREEN, "Game Title Here"))
    {
        std::cout << "Failed to create window" << std::endl;
        return 1;
    }

    Game* game = new Game();
    if(!game->init(WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        std::cout << "Game class init failed" << std::endl;
        return 1;
    }

    game->run();
    return 0;
}
