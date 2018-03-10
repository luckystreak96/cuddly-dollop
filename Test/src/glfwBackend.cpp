#include "glfwBackend.h" // include this first or get gl b4 glew error
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <vector>
#include <algorithm>
#include "game.h"
#include "input_manager.h"
#include "gameData.h"

GLFWwindow* GLFWManager::m_window = NULL;
Vector2f GLFWManager::_mngrGLVersion = Vector2f(2, 0);
bool GLFWManager::_joyStickMode = false;

void Resize(GLFWwindow* window)
{
	const int width = 16;
	const int height = 9;
	const int size = 32;
	float multiplierx = 1;
	float multipliery = 1;
	int multiplierFinal = 1;
	//Setup viewport to fit the window size
	int screenW, screenH;
	glfwGetWindowSize(window, &screenW, &screenH);

	multiplierx = (float)screenW / 480.f;
	multipliery = (float)screenH / 270.f;
	multiplierFinal = (int)(fmin(multiplierx, multipliery));

	int viewW, viewH;
	viewW = multiplierFinal * 480;
	viewH = multiplierFinal * 270;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	int w = mode->width;
	int h = mode->height;

	// Fullscreen mode
	if (screenW == w && screenH == h)
	{
		viewH = h;
		viewW = w;
	}
	glViewport((screenW - viewW) / 2, (screenH - viewH) / 2, (GLsizei)(viewW), (GLsizei)(viewH));
	OrthoProjInfo::GetRegularInstance().Bottom = -(viewH / 2.0f);
	OrthoProjInfo::GetRegularInstance().Top = (viewH / 2.0f);
	OrthoProjInfo::GetRegularInstance().Left = -(viewW / 2.0f);
	OrthoProjInfo::GetRegularInstance().Right = (viewW / 2.0f);
	OrthoProjInfo::GetRegularInstance().Size = size * multiplierFinal;
	OrthoProjInfo::GetRegularInstance().changed = true;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GameData::KeyMap.at(A_Exit) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_F11 && action == GLFW_RELEASE)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, std::get<bool>(GameData::Options.at("fullscreen")) ? NULL : monitor, 5, 35, mode->width - 10, mode->height - 80, mode->refreshRate);
		GameData::Options.at("fullscreen") = !std::get<bool>(GameData::Options.at("fullscreen"));
		glfwSwapInterval(1);
	}

	KeyStatus status;
	if (action == GLFW_PRESS)
		status = KeyPressed;
	else if (action == GLFW_RELEASE)
		status = Release;
	else if (action == GLFW_REPEAT)
		status = KeyPressed;

	InputManager::GetInstance().Input((unsigned int)key, status);

	GLFWManager::_joyStickMode = false;
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
	Resize(window);
}

GLFWManager::GLFWManager()
{
	// Init glfw
	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw" << std::endl;
		std::getchar();
		exit(1);
	}


	// Get screen info
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	m_screenWidth = mode->width;
	m_screenHeight = mode->height;
	m_refreshRate = mode->refreshRate;

	std::vector<Vector2f> versions{ Vector2f(4, 6), Vector2f(3, 3), Vector2f(2, 0), Vector2f(1, 1) };

	// Try the different GL contexts
	for (auto x : versions)
	{
		// GL Version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, x.x);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, x.y);

		// Create window
		m_window = glfwCreateWindow(m_screenWidth - 10, m_screenHeight - 80, "Cuddly-Dollop", NULL, NULL);
		if (!m_window)
		{
			std::cout << "Window failed to be created on context: " << x.x << "." << x.y << std::endl;
			if (x == versions.at(versions.size() - 1))
			{
				std::cout << "OpenGL version not supported, press any key to exit..." << std::endl;
				std::getchar();
				exit(1);
			}
			continue;
		}
		std::cout << "GL context opened on OpenGL version " << x.x << "." << x.y << std::endl;
		// Set the gl version to be accessed from the effects
		_mngrGLVersion = x;
		break;
	}

	// Fullscreen / windowed initialization
	if (std::get<bool>(GameData::Options.at("fullscreen")) == true)
	{
		glfwSetWindowMonitor(m_window, monitor, 5, 35, m_screenWidth - 10, m_screenHeight - 80, m_refreshRate);
	}
	else
	{
		glfwHideWindow(m_window);
		glfwSetWindowPos(m_window, 5, 35);
		glfwShowWindow(m_window);
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

	// GL context
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

	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	glfwSwapInterval(1);

	printf("GL version: %s\n", glGetString(GL_VERSION));

	Resize(m_window);
}

void GLFWManager::GLFWMainLoop(Game* game)
{
	// Game init
	if (!game->init(_mngrGLVersion))
	{
		std::cout << "Game class init failed" << std::endl;
		std::getchar();
		exit(1);
	}

	// Main loop
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		HandleJoystickInput();
		game->renderSceneCB();
		glfwSwapBuffers(m_window);
	}

	// Destroy GLFW context
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void GLFWManager::HandleJoystickInput()
{
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	if (!present)
		return;

	int count;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
	InputManager::GetInstance().SetKeyPercent(A_Left, _joyStickMode ? axes[0] : 1);
	InputManager::GetInstance().SetKeyPercent(A_Right, _joyStickMode ? axes[0] : 1);
	InputManager::GetInstance().SetKeyPercent(A_Up, _joyStickMode ? axes[1] : 1);
	InputManager::GetInstance().SetKeyPercent(A_Down, _joyStickMode ? axes[1] : 1);

	// All the values are between -1 and 1
	// Axes 0-1 = X-Y for left stick
	// 2-3 = X-Y for right stick
	// 4-5 = L2 R2 pressure

	// Dead zone
	float deadZone = 0.15f;

	static float prevX = 0;
	static float prevY = 0;
	static std::vector<bool> buttonStates{ false, false };

	// Left stick X
	if (abs(axes[0]) >= deadZone)
	{
		// Left
		if (axes[0] < 0)
		{
			InputManager::GetInstance().Input(GameData::KeyMap.at(A_Left), true);
			if (prevX > deadZone)
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Right), false);
		}
		// Right
		else
		{
			InputManager::GetInstance().Input(GameData::KeyMap.at(A_Right), true);
			if (abs(prevX) > deadZone)
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Left), false);
		}
		GLFWManager::_joyStickMode = true;
	}
	else
	{
		// If there was a direction last frame, now its released
		if (abs(prevX) > deadZone)
		{
			if (prevX < 0)
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Left), false);
			else
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Right), false);
		}
	}


	// Left stick Y
	if (abs(axes[1]) >= deadZone)
	{
		// Down
		if (axes[1] < 0)
		{
			InputManager::GetInstance().Input(GameData::KeyMap.at(A_Down), true);
			if (prevY >= deadZone)
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Up), false);
		}
		else
		{
			InputManager::GetInstance().Input(GameData::KeyMap.at(A_Up), true);
			if (abs(prevY) > deadZone && prevY < 0)
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Down), false);
		}
		GLFWManager::_joyStickMode = true;
	}
	else
	{
		if (abs(prevY) > deadZone)
		{
			if (prevY < 0)
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Down), false);
			else
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Up), false);
		}
	}

	int buttonCount;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);


	// 'A' button is buttons[0]
	if (buttons[0] == GLFW_PRESS)
		InputManager::GetInstance().Input(GameData::KeyMap.at(A_Accept), true);
	else if (buttonStates[0])
		InputManager::GetInstance().Input(GameData::KeyMap.at(A_Accept), false);

	// 'B' button is buttons[1]
	if (buttons[1] == GLFW_PRESS)
		InputManager::GetInstance().Input(GameData::KeyMap.at(A_Cancel), true);
	else if (buttonStates[1])
		InputManager::GetInstance().Input(GameData::KeyMap.at(A_Cancel), false);

	prevX = axes[0];
	prevY = axes[1];
	buttonStates[0] = buttons[0];
	buttonStates[1] = buttons[1];
}
