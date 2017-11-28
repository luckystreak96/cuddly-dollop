#include "game.h"

Game::Game() : m_numFrames(0), m_exit(false)
{
}

Game::~Game()
{
}

bool Game::init(float width, float height)
{
	SetupTextureAtlas();

	Model::GetInstance().init("res/models/models.data");

	//Setup viewport to fit the window size
	glViewport(0, 0, (GLsizei)(glutGet(GLUT_WINDOW_WIDTH)), (GLsizei)(glutGet(GLUT_WINDOW_HEIGHT)));

	std::shared_ptr<SceneWorld> world = std::shared_ptr<SceneWorld>(new SceneWorld(1));
	SceneManager::GetInstance().SetScene(world);

	OrthoProjInfo::GetRegularInstance().Bottom = -512.0f;
	OrthoProjInfo::GetRegularInstance().Top = 512.0f;
	OrthoProjInfo::GetRegularInstance().Left = -960.0f;
	OrthoProjInfo::GetRegularInstance().Right = 960.f;
	OrthoProjInfo::GetRegularInstance().zNear = 100.f;
	OrthoProjInfo::GetRegularInstance().zFar = -100.f;
	OrthoProjInfo::GetRegularInstance().Size = 64.f;
	OrthoProjInfo::GetRegularInstance().changed = true;

	return true;
}

// Finds all the png files to make the texture atlas for tiles/particles
void Game::SetupTextureAtlas()
{
	std::vector<std::string> vs;
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	hFind = FindFirstFile(L"res/sprites/tiles/*.png", &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			std::wstring w = FindFileData.cFileName;
			vs.push_back("res/sprites/tiles/" + std::string(w.begin(), w.end()));
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
	hFind = FindFirstFile(L"res/sprites/particles/*.png", &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			std::wstring w = FindFileData.cFileName;
			vs.push_back("res/sprites/particles/" + std::string(w.begin(), w.end()));
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}

	for (std::string s : vs)
		TextureAtlas::m_textureAtlas.AddTexture(s);
	TextureAtlas::m_textureAtlas.Generate(32, false, "res/tiles.png");
	TextureAtlas::m_textureAtlas.ShortenTextureList();
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
	switch (key)
	{
	case GLUT_KEY_F3://ESC
		InputManager::GetInstance().SetLockLevel(0);
		break;
	case GLUT_KEY_F1:
		Globals::DEBUG_DRAW_NORMALS = !Globals::DEBUG_DRAW_NORMALS;
		break;
	case GLUT_KEY_F2:
		Globals::DEBUG_DRAW_TILE_OUTLINES = !Globals::DEBUG_DRAW_TILE_OUTLINES;
		break;
	case GLUT_KEY_F11:
		if (m_isFullscreen)
		{
			glutReshapeWindow(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
			glutPositionWindow(0, 0);
			m_isFullscreen = false;
		}
		else
		{
			glutFullScreen();
			m_isFullscreen = true;
		}
	//case GLUT_KEY_F10:
	//	glutReshapeWindow(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	//	glutPositionWindow(0, 0);
	//	m_isFullscreen = false;
	default:
		break;
	}
}

void Game::windowResizeCB(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	OrthoProjInfo::GetRegularInstance().Bottom = -(h / 2.0f);
	OrthoProjInfo::GetRegularInstance().Top = (h / 2.0f);
	OrthoProjInfo::GetRegularInstance().Left = -(w / 2.0f);
	OrthoProjInfo::GetRegularInstance().Right = (w / 2.0f);
	OrthoProjInfo::GetRegularInstance().changed = true;
}