#include "glfwBackend.h"
#include "define_gl.h"
#include "game.h"
#include "input_manager.h"
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

static bool FullScreen = true;
GLFWwindow* GLFWManager::m_window = NULL;;

void Resize(GLFWwindow* window)
{
	//Setup viewport to fit the window size
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	glViewport(0, 0, (GLsizei)(w), (GLsizei)(h));
	OrthoProjInfo::GetRegularInstance().Bottom = -(h / 2.0f);
	OrthoProjInfo::GetRegularInstance().Top = (h / 2.0f);
	OrthoProjInfo::GetRegularInstance().Left = -(w / 2.0f);
	OrthoProjInfo::GetRegularInstance().Right = (w / 2.0f);
	OrthoProjInfo::GetRegularInstance().changed = true;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_F11 && action == GLFW_RELEASE)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, FullScreen ? NULL : monitor, 1, 30, mode->width, mode->height, mode->refreshRate);
		FullScreen = !FullScreen;
	}

	InputManager::GetInstance().Input((unsigned int)key, action);
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
	Resize(window);
}

GLFWManager::GLFWManager()
{
	// Init glfw
	if(!glfwInit())
	{
	    std::cout << "Failed to initialize glfw" << std::endl;
		std::getchar();
	    exit(1);
	}

	// GL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Create window
	m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cuddly-Dollop", NULL, NULL);
	if (!m_window)
	{
		std::cout << "Window failed to be created!" << std::endl;
		std::getchar();
		exit(1);
	}

	// Window icon
	std::string text = "res/sprites/special/ghost_shiny_blue.png";
	ResourceManager::GetInstance().LoadTexture(text);
	ResourceManager::GetInstance().LoadGLTexture(text);
	Texture* tex = ResourceManager::GetInstance().GetTexture(text);
	GLFWimage image = GLFWimage();
	image.width = 32;
	image.height = 32;
	image.pixels = tex->GetPixels();

	glfwSetWindowIcon(m_window, 1, &image);

	// Set fullscreen, call again with null monitor to set windowed
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	m_screenWidth = mode->width;
	m_screenHeight = mode->height;
	m_refreshRate = mode->refreshRate;

	glfwSetWindowMonitor(m_window, monitor, 10, 10, m_screenWidth, m_screenHeight, m_refreshRate);

	glfwMakeContextCurrent(m_window);

	// Key callback
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, window_size_callback);

	// GLEW init
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cout << "GLEW error: " << glewGetErrorString(res) << std::endl;
		std::getchar();
		exit(1);
	}

	glClearStencil(0x0);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilMask(0x00);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	glfwSwapInterval(1);

	printf("GL version: %s\n", glGetString(GL_VERSION));

	Resize(m_window);
}

void GLFWManager::GLFWMainLoop()
{
	Game* game = new Game();
	if (!game->init(WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		std::cout << "Game class init failed" << std::endl;
		std::getchar();
		exit(1);
	}

	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		game->renderSceneCB();
		glfwSwapBuffers(m_window);
	}

	glfwDestroyWindow(m_window);
	glfwTerminate();
}
