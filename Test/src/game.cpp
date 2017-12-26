#include "game.h"
#include "gameData.h"
#include "textureatlas.h"
#include <GLFW\glfw3.h>

Game::Game() : m_numFrames(0), m_exit(false), m_muted(true)
{
}

Game::~Game()
{
}

bool Game::init(float width, float height)
{
	GameData::LoadFromFile();
	SetupTextureAtlas();

	// Mute by default
	SoundManager::GetInstance().SetMasterVolume(m_muted ? 0.f : 1.f);

	Model::GetInstance().init("res/models/models.data");

	std::shared_ptr<SceneWorld> world = std::shared_ptr<SceneWorld>(new SceneWorld(1));
	SceneManager::GetInstance().SetScene(world);

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

void Game::renderSceneCB()
{
	HandleInput();

	if (m_exit)
	{
		GameData::SaveToFile();
		return;
	}

	//FPS
	ElapsedTime::GetInstance().CalculateElapsedTime();
	if (ElapsedTime::GetInstance().GetElapsedTime() == -1)
		return;

	//std::cout << ElapsedTime::GetInstance().GetFPS() << std::endl;

	SceneManager::GetInstance().Act();
}

void Game::HandleInput()
{
	// Draw normals
	if (InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_F1, KeyStatus::AnyRelease))
		Globals::DEBUG_DRAW_NORMALS = !Globals::DEBUG_DRAW_NORMALS;

	// Draw outlines
	if (InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_F2, KeyStatus::AnyRelease))
		Globals::DEBUG_DRAW_TILE_OUTLINES = !Globals::DEBUG_DRAW_TILE_OUTLINES;

	// Reset lock level
	if (InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_F3, KeyStatus::AnyRelease))
		InputManager::GetInstance().SetLockLevel(0);

	// Mute
	if (InputManager::GetInstance().FrameKeyStatus(77, KeyStatus::AnyRelease))
		MuteButton();

}

void Game::MuteButton()
{
	m_muted = !m_muted;
	SoundManager::GetInstance().SetMasterVolume(m_muted ? 0.f : 1.f);
}

