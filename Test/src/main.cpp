#include <iostream>
#include "glfwBackend.h"
#include "game.h"
#include "gameData.h"

int main(int argc, char** argv)
{
	Game* game = new Game();
	GLFWManager manager = GLFWManager();

#ifdef NDEBUG
	FreeConsole();
#endif
	manager.GLFWMainLoop(game);

	delete game;
    return 0;
}
