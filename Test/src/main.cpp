#include <iostream>
#include "glfwBackend.h"
#include "game.h"
//#include <GLFW\glfw3.h>

int main(int argc, char** argv)
{
	GLFWManager manager = GLFWManager();
	FreeConsole();
	manager.GLFWMainLoop();

    return 0;
}
