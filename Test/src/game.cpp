#include "game.h"

Game::Game() : m_numFrames(0), m_exit(false)
{
}

Game::~Game()
{
}

bool Game::init(float width, float height)
{
	Model::GetInstance().init("res/models.data");

	SceneManager::GetInstance().SetScene(&SceneWorld::GetInstance());

	int hheight = glutGet(GLUT_WINDOW_HEIGHT);
	int wwidth = glutGet(GLUT_WINDOW_WIDTH);

	//m_shadowMapSize = (float)(wwidth > hheight ? wwidth : hheight);
	//if (!ShadowMapFBO::GetInstance().Init((unsigned int)m_shadowMapSize, (unsigned int)m_shadowMapSize))
	//	return false;

	OrthoProjInfo::GetRegularInstance().Bottom = ORTHO_BOTTOM;//-(hheight / 64 / 2);// divide by 2 cuz yeah
	OrthoProjInfo::GetRegularInstance().Top = ORTHO_TOP;//(hheight / 64 / 2);
	OrthoProjInfo::GetRegularInstance().Left = ORTHO_LEFT;//-(wwidth / 64 / 2);
	OrthoProjInfo::GetRegularInstance().Right = ORTHO_RIGHT;//(wwidth / 64 / 2);
	OrthoProjInfo::GetRegularInstance().zNear = ORTHO_NEAR;
	OrthoProjInfo::GetRegularInstance().zFar = ORTHO_FAR;

	PersProjInfo::GetRegularInstance().FOV = 30.0f;
	PersProjInfo::GetRegularInstance().Width = width;
	PersProjInfo::GetRegularInstance().Height = height;

	PersProjInfo::GetShadowInstance().FOV = 90.0f;
	PersProjInfo::GetShadowInstance().Width = m_shadowMapSize;
	PersProjInfo::GetShadowInstance().Height = m_shadowMapSize;

	//glEnable(GL_TEXTURE_CUBE_MAP);

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

	//std::cout << ElapsedTime::GetInstance().GetFPS() << std::endl;

	SceneManager::GetInstance().Act();
}

void Game::specialKeyboardCB(int key, int x, int y)
{	
	switch (key)
	{
	case GLUT_KEY_F1:
		Globals::DEBUG_DRAW_NORMALS = !Globals::DEBUG_DRAW_NORMALS;
		break;
	case GLUT_KEY_F2:
		Globals::DEBUG_DRAW_TILE_OUTLINES = !Globals::DEBUG_DRAW_TILE_OUTLINES;
		break;
	case GLUT_KEY_F11:
		if (m_isFullscreen)
		{
			glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
			glutPositionWindow(0, 0);
			m_isFullscreen = false;
		}
		else
		{
			glutFullScreen();
			m_isFullscreen = true;
		}
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
	//float w = (float)glutGet(GLUT_WINDOW_WIDTH);
	//float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
	//float r = (float)w / (float)h;
		//Prevents stretching
	//glViewport(0, 0, WINDOW_WIDTH * ratio, WINDOW_HEIGHT * ratio);

	//Stretches to fit window size
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	//glViewport(0, 0, (GLsizei)(WINDOW_WIDTH * RATIO), (GLsizei)(WINDOW_HEIGHT * RATIO));

	//OrthoProjInfo::GetRegularInstance().Bottom = -(h);
	//OrthoProjInfo::GetRegularInstance().Top = (h);
	//OrthoProjInfo::GetRegularInstance().Left = -(w * r);
	//OrthoProjInfo::GetRegularInstance().Right = (w * r);

}