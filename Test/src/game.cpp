#include "game.h"

Game::Game() : m_numFrames(0), m_exit(false)
{
}

Game::~Game()
{
}

bool Game::init(float width, float height)
{
	SceneManager::GetInstance().SetScene(&SceneWorld::GetInstance());

	m_shadowMapSize = glutGet(GLUT_WINDOW_WIDTH) > glutGet(GLUT_WINDOW_HEIGHT) ? glutGet(GLUT_WINDOW_WIDTH) : glutGet(GLUT_WINDOW_HEIGHT);
	if (!ShadowMapFBO::GetInstance().Init(m_shadowMapSize, m_shadowMapSize))
		return false;

	OrthoProjInfo::GetRegularInstance().Bottom = -9;
	OrthoProjInfo::GetRegularInstance().Top = 9;// glutGet(GLUT_WINDOW_HEIGHT);
	OrthoProjInfo::GetRegularInstance().Left = -16;
	OrthoProjInfo::GetRegularInstance().Right = 16;// glutGet(GLUT_WINDOW_WIDTH);
	OrthoProjInfo::GetRegularInstance().zNear = -10;
	OrthoProjInfo::GetRegularInstance().zFar = 10;

	PersProjInfo::GetRegularInstance().FOV = 30.0f;
	PersProjInfo::GetRegularInstance().Width = width;
	PersProjInfo::GetRegularInstance().Height = height;

	PersProjInfo::GetShadowInstance().FOV = 90.0f;
	PersProjInfo::GetShadowInstance().Width = m_shadowMapSize;
	PersProjInfo::GetShadowInstance().Height = m_shadowMapSize;

	glEnable(GL_TEXTURE_CUBE_MAP);

	return true;
}

void Game::run()
{
	GLUTBackendRun(this);
}

void Game::renderSceneCB()
{
	if (m_exit)
		glutLeaveMainLoop();

	//FPS
	ElapsedTime::GetInstance().CalculateElapsedTime();
	if (ElapsedTime::GetInstance().GetElapsedTime() == -1)
		return;

	///If ever i want to set up frame by frame again...
	////RENDER SETUP
	//if (!m_pause || m_numFrames > 0)
	//{
	//	//If we pause to slowly pass frames...
	//	if (m_numFrames > 0)
	//	{
	//		m_numFrames--;
	//		//...then set the elapsedtime to a reasonable amount (60 fps)
	//		ElapsedTime::GetInstance().SetBufferElapsedTime();
	//	}
	//	Update();
	//}

	SceneManager::GetInstance().Act();
}

void Game::specialKeyboardCB(int key, int x, int y)
{	
	switch (key)
	{
	case GLUT_KEY_F1://ESC
		Globals::DEBUG_DRAW_NORMALS = !Globals::DEBUG_DRAW_NORMALS;
		break;
	default:
		break;
	}

	InputManager::GetInstance().SpecialInput(key, true);
}

void Game::keyboardCB(unsigned char key, int x, int y)//x and y are mouse pos
{
	switch (key)
	{
	case 27://ESC
		m_exit = true;
		break;
	}
	InputManager::GetInstance().Input(key, true);
	//m_camera->OnKeyboard(key);
	//switch (key)
	//{
	//case 32://SPACE
	//	//m_player->OnKeyboard(key, false);
	//	break;
	////case 'p':
	////	m_pause = !m_pause;
	////	break;
	////case 'o':
	////	m_numFrames = SKIP_FRAMES;
	////	break;
	////case 'c':
	////	m_camAngle += 0.5f;
	////	break;
	//case 'x':
	//	//m_camAngle -= 0.5f;
	//	break;
	//default:
	//	break;
	//}
}

void Game::keyboardUpCB(unsigned char key, int x, int y)
{
	InputManager::GetInstance().Input(key, false);
	//switch (key)
	//{
	//case 27://ESC
	//	glutLeaveMainLoop();
	//	break;
	//case 32://SPACE
	//	//m_player->OnKeyboard(key, true);
	//	break;
	//default:
	//	break;
	//}
}

void Game::specialKeyboardUpCB(int key, int x, int y)
{
	InputManager::GetInstance().SpecialInput(key, false);
}

void Game::windowResizeCB(int w, int h)
{
	PersProjInfo::GetRegularInstance().Width = glutGet(GLUT_WINDOW_WIDTH);
	PersProjInfo::GetRegularInstance().Height = glutGet(GLUT_WINDOW_HEIGHT);
	m_shadowMapSize = glutGet(GLUT_WINDOW_WIDTH) > glutGet(GLUT_WINDOW_HEIGHT) ? glutGet(GLUT_WINDOW_WIDTH) : glutGet(GLUT_WINDOW_HEIGHT);
	ShadowMapFBO::GetInstance().Init(m_shadowMapSize, m_shadowMapSize);
}