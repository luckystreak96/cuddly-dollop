#include <iostream>
#include "glfwBackend.h"
#include "game.h"
//#include <GLFW\glfw3.h>

int main(int argc, char** argv)
{
    //GLUTBackendInit(argc, argv, false, false);

    //if(!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Cuddly-Dollop"))
    //{
    //    std::cout << "Failed to create window" << std::endl;
    //    return 1;
    //}

	GLFWManager manager = GLFWManager();




	manager.GLFWMainLoop();

    //game->run();

	//glfwDestroyWindow(window);
	//glfwTerminate();

    return 0;
}
