#ifndef GLFWBACKEND_H__
#define GLFWBACKEND_H__

#include <iostream>
#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#include <GLFW\glfw3.h>

class GLFWManager
{
public:
	GLFWManager();
	void GLFWMainLoop();
	void Resize();
private:
	GLFWwindow* m_window;
	int m_screenWidth, m_screenHeight;
};

#endif
