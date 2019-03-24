#include "glfwBackend.h" // include this first or get gl b4 glew error

#include "game.h"
#include "input_manager.h"
#include "gameData.h"
#include "resource_manager.h"
#include "mathutils.h"
#include "projection.h"
#include "renderer.h"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <vector>
#include <algorithm>
#include <algorithm>
#include <assert.h>
#include <thread>

GLFWwindow* GLFWManager::m_window = NULL;
Vector2f GLFWManager::_mngrGLVersion = Vector2f(2, 0);
bool GLFWManager::_joyStickMode = false;
bool GLFWManager::_swapInterval = 1;

void Resize(GLFWwindow* window)
{
	const int offsetX = 480;
	const int offsetY = 270;
	const int width = 16;
	const int height = 9;
	float multiplierx = 1;
	float multipliery = 1;
	int multiplierFinal = 1;
	//Setup viewport to fit the window size
	int screenW, screenH;
	glfwGetWindowSize(window, &screenW, &screenH);

	multiplierx = (float)screenW / (float)offsetX;
	multipliery = (float)screenH / (float)offsetY;
	multiplierFinal = (int)(fmin(multiplierx, multipliery));

	// This allows getting the resolution from file and keeping that resolution in fullscreen in case of slow GPU
	// Delete this line to go back to resolution based on window size
	multiplierFinal = std::get<int>(GameData::Options.at("resolution"));

	int viewW, viewH;
	//viewW = multiplierFinal * 480;
	//viewH = multiplierFinal * 270;
	viewW = 1024;
	viewH = 768;


	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	int w = mode->width;
	int h = mode->height;

	// Fullscreen mode
	if (std::get<bool>(GameData::Options.at("fullscreen")))
	{
		//viewH = multiplierFinal * offsetY;
		//viewW = multiplierFinal * offsetX;
		viewH = h;
		viewW = w;
		std::cout << "Switched to fullscreen mode : Width - " << viewW << " Height - " << viewH << std::endl;
		//glfwSetWindowSize(window, viewW, viewH);
	}
	else
	{
		//viewH = multiplierFinal * offsetY;
		//viewW = multiplierFinal * offsetX;
		glfwSetWindowSize(GLFWManager::m_window, viewW, viewH);
		w = viewW;
		h = viewH;
		std::cout << "Switched to windowed mode : Width - " << viewW << " Height - " << viewH << std::endl;
	}
	// The next comment centers the viewport if the window is too big for the resolution
	glViewport(0, 0, (GLsizei)(viewW), (GLsizei)(viewH));
	OrthoProjInfo::GetRegularInstance().Bottom = -(h / 2.0f);
	OrthoProjInfo::GetRegularInstance().Top = (h / 2.0f);
	OrthoProjInfo::GetRegularInstance().Left = -(w / 2.0f);
	OrthoProjInfo::GetRegularInstance().Right = (w / 2.0f);
	OrthoProjInfo::GetRegularInstance().Size = 64;
	OrthoProjInfo::GetRegularInstance().changed = true;

	PersProjInfo::GetRegularInstance().Height = h;
	PersProjInfo::GetRegularInstance().Width = w;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GameData::KeyMap.at(A_Exit) && action == GLFW_PRESS)
		GLFWManager::Exit();

	//if (key == GLFW_KEY_F9 && action == GLFW_RELEASE)
	//	Transformation::perspectiveOrtho = !Transformation::perspectiveOrtho;
	if (key == GLFW_KEY_F6 && (action == GLFW_PRESS || action == GLFW_REPEAT))
		Camera::_currentCam->_3dTarget.x > 4.0f ? Camera::_currentCam->_3dTarget.x = 0.0f : Camera::_currentCam->_3dTarget.x += 0.1f;
	if (key == GLFW_KEY_F7 && (action == GLFW_PRESS || action == GLFW_REPEAT))
		Camera::_currentCam->_3dTarget.y > 4.0f ? Camera::_currentCam->_3dTarget.y = 0.0f : Camera::_currentCam->_3dTarget.y += 0.1f;
	if (key == GLFW_KEY_F8 && (action == GLFW_PRESS || action == GLFW_REPEAT))
		Camera::_currentCam->_3dTarget.z > 4.0f ? Camera::_currentCam->_3dTarget.z = 0.0f : Camera::_currentCam->_3dTarget.z += 0.1f;

	//if (key == GLFW_KEY_F2 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	//	GraphicsComponent::_persRotation += 0.1f;
	//if (key == GLFW_KEY_F3 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	//	GraphicsComponent::_persRotation -= 0.1f;


	if (key == GLFW_KEY_F11 && action == GLFW_RELEASE)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		GameData::Options.at("fullscreen") = !std::get<bool>(GameData::Options.at("fullscreen"));

		if (std::get<bool>(GameData::Options.at("fullscreen")))
		{
			int multiplierFinal = std::get<int>(GameData::Options.at("resolution"));

			int viewW, viewH;
			viewW = multiplierFinal * 480;
			viewH = multiplierFinal * 270;
			glfwSetWindowMonitor(window, monitor, 0, 0, viewW, viewH, mode->refreshRate);
		}
		else
			glfwSetWindowMonitor(window, NULL, 5, 35, mode->width - 10, mode->height - 80, mode->refreshRate);

		glfwSwapInterval(GLFWManager::_swapInterval);
		//glfwSwapInterval(GLFWManager::_swapInterval);
	}
	if (key == GLFW_KEY_F10 && action == GLFW_RELEASE)
	{
		GLFWManager::_swapInterval = GLFWManager::_swapInterval == 0 ? 1 : 0;
		glfwSwapInterval(GLFWManager::_swapInterval);
	}

	if(action == GLFW_RELEASE) {
		if (key == GLFW_KEY_ENTER)
			InputManager::GetInstance().InputText((unsigned int) '\n');
		else if (key == GLFW_KEY_BACKSPACE)
			InputManager::GetInstance().InputText((unsigned int) '\b');
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

static void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    InputManager::GetInstance().InputText(codepoint);
	GLFWManager::_joyStickMode = false;
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
	Resize(window);
}

void ETB_GL_ERROR_CALLBACK(GLenum        source,
	GLenum        type,
	GLuint        id,
	GLenum        severity,
	GLsizei       length,
	const GLchar* message,
	GLvoid*       userParam)
{
	std::cout << "OpenGL Error:" << std::endl;
	std::cout << "=============\n" << std::endl;

	std::cout << " Object ID: " << std::endl;
	std::cout << id << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;

	std::cout << " Message:   " << std::endl;
	std::cout << message << std::endl << std::endl;

	// Trigger a breakpoint in gDEBugger...
	glFinish();

	// Trigger a breakpoint in traditional debuggers...
	//assert(false);
}

void GLFWManager::Exit()
{
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
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

	std::vector<Vector2f> versions{ Vector2f(4, 3), Vector2f(3, 3), Vector2f(2, 0), Vector2f(1, 1) };

	// Try the different GL contexts
	for (auto x : versions)
	{
		// GL Version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, x.x);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, x.y);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);


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
	glfwSetCharCallback(m_window, character_callback);
	glfwSetWindowSizeCallback(m_window, window_size_callback);

	// GLEW init
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cout << "GLEW error: " << glewGetErrorString(res) << std::endl;
		std::getchar();
		exit(1);
	}


	if (true && glDebugMessageCallbackARB != NULL) {
		// Debug Output is supported, hooray!
		//glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, NULL, GL_TRUE);
		//glDebugMessageControlARB(GL_DEBUG_SOURCE_OTHER, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);// forces the pipeline to flush every line so debugging isnt total garbage
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		glDebugMessageCallbackARB((GLDEBUGPROCARB)ETB_GL_ERROR_CALLBACK, NULL);
	}

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	glClearStencil(0x0);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilMask(0x00);

	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	glfwSwapInterval(GLFWManager::_swapInterval);

	printf("GL version: %s\n", glGetString(GL_VERSION));

	printf("\nSpecial keys:\n");
	printf("-------------\n");
	printf("F%i - %s", 10, "Toggle vertical sync\n");
	printf("F%i - %s", 11, "Toggle fullscreen\n");

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

	//std::thread draw_thread;

	// Main loop
	while (!glfwWindowShouldClose(m_window))
	{
		//static int counter = 5;
		glfwPollEvents();
		HandleJoystickInput();
		game->renderSceneCB();
		//counter--;
		//if (counter > 0)
		//if (draw_thread.joinable())
			//draw_thread.join();
		//draw_thread = std::thread(glfwSwapBuffers, m_window);
		//glFlush();
		//Sleep(1);
		//glFinish();
		//Renderer::GetInstance().SwapBuffers(m_window);
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
	float deadZone = -0.8f;

	static float prevR2 = 0;
	if (axes[5] > deadZone)
		InputManager::GetInstance().Input(GameData::KeyMap.at(A_AltR), true);
	else if (prevR2 >= deadZone && axes[5] < deadZone)
		InputManager::GetInstance().Input(GameData::KeyMap.at(A_AltR), false);

	//std::cout << axes[5] << std::endl;

	deadZone = 0.15f;

	static float prevX = 0;
	static float prevY = 0;
	static std::vector<bool> buttonStates(20);
	// Left stick X
	if (abs(axes[0]) >= deadZone)
	{
		// Left
		if (axes[0] < 0)
		{
			InputManager::GetInstance().Input(GameData::KeyMap.at(A_Left), true);
			//if (prevX > deadZone)
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Right), false);
		}
		// Right
		else
		{
			InputManager::GetInstance().Input(GameData::KeyMap.at(A_Right), true);
			//if (abs(prevX) > deadZone)
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Left), false);
		}
		GLFWManager::_joyStickMode = true;
	}
	else
	{
		// If there was a direction last frame, now its released
		if (abs(prevX) > deadZone)
		{
			//if (prevX < 0)
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Left), false);
			//else
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
			//if (prevY >= deadZone)
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Up), false);
		}
		else
		{
			InputManager::GetInstance().Input(GameData::KeyMap.at(A_Up), true);
			//if (abs(prevY) > deadZone && prevY < 0)
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Down), false);
		}
		GLFWManager::_joyStickMode = true;
	}
	else
	{
		if (abs(prevY) > deadZone)
		{
			//if (prevY < 0)
				InputManager::GetInstance().Input(GameData::KeyMap.at(A_Down), false);
			//else
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

	// 'Start' button is buttons[7]
	if (buttons[7] == GLFW_PRESS)
		InputManager::GetInstance().Input(GameData::KeyMap.at(A_Menu), true);
	else if (buttonStates[7])
		InputManager::GetInstance().Input(GameData::KeyMap.at(A_Menu), false);

	// 'R1' button is buttons[5]
	//if (buttons[5] == GLFW_PRESS)
	//	InputManager::GetInstance().Input(GameData::KeyMap.at(A_AltR), true);
	//else if (buttonStates[5])
	//	InputManager::GetInstance().Input(GameData::KeyMap.at(A_AltR), false);

	// For a visual on which buttons are which index

	//for(int i = 0; i < buttonCount; i++)
	//	std::cout << i << " -> " << buttons[i];
	//std::cout << std::endl;

	prevX = axes[0];
	prevY = axes[1];
	prevR2 = axes[5];
	for (int i = 0; i < buttonCount; i++)
		buttonStates[i] = buttons[i];
	//buttonStates[0] = buttons[0];
	//buttonStates[1] = buttons[1];
}
