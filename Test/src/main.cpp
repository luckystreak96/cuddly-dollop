#include <iostream>
#include "glfwBackend.h"
#include "game.h"
//#include <GLFW\glfw3.h>

#include "eventFactory.h"

int main(int argc, char** argv)
{
	EventTest ev, ev2;
	ev2.inner = true;
	std::map<std::string, EventTest> map;
	map.emplace("test", ev2);
	ev.inner = map;

	std::cout << std::get<bool>(std::get<std::map<std::string, EventTest>>(ev.inner).at("test").inner) << std::endl;


	GLFWManager manager = GLFWManager();

#ifdef NDEBUG
	FreeConsole();
#endif

	manager.GLFWMainLoop();

    return 0;
}
