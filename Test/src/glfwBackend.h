#ifndef GLFWBACKEND_H__
#define GLFWBACKEND_H__

#include <iostream>
#include <GL/glew.h>
#include <GLFW\glfw3.h>

class GLFWManager
{
public:
	GLFWManager();
	void GLFWMainLoop();
public:
	static GLFWwindow* m_window;
private:
	int m_screenWidth, m_screenHeight, m_refreshRate;
};

#endif
