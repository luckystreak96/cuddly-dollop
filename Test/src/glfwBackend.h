#ifndef GLFWBACKEND_H__
#define GLFWBACKEND_H__

#include <iostream>
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "vector3f.h"

class Game;

class GLFWManager
{
public:
	GLFWManager();
	void GLFWMainLoop(Game* game);
	void HandleJoystickInput();
public:
	static GLFWwindow* m_window;
	static Vector2f _mngrGLVersion;
	static bool _joyStickMode;

private:
	int m_screenWidth, m_screenHeight, m_refreshRate;
};

#endif
