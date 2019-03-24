#include "game.h"

#include "gameData.h"
#include "localizationData.h"
#include "textureatlas.h"
#include "vector3f.h"
#include "effect.h"
#include "FBO.h"
#include "soundManager.h"
#include "scene_manager.h"
#include "scene_world.h"
#include "animation.h"

#include <GLFW/glfw3.h>
#ifndef _WIN32
#include <dirent.h>
#endif

Game::Game() : m_exit(false)
{
	GameData::LoadSettings();
	LocalizationData::LoadLocalization();
	Model::GetInstance().init("res/models/models.data");
}

Game::~Game()
{
}

bool Game::init(Vector2f version)
{
	// Settings decide 3d or 2d
	Transformation::perspectiveOrtho = !std::get<bool>(GameData::Options.at("3d"));
	Animation::SetupAnimationMetaData();
	GameData::LoadGameData();
	GameData::NewGame();

	FBO::_fboGLVersion = version;
	Effect::_efctGLVersion = version;
	SetupTextureAtlas();

	// Mute by default
	SoundManager::GetInstance().SetMasterVolume(std::get<bool>(GameData::Options.at("mute")) ? 0.f : 1.f);

	std::shared_ptr<SceneWorld> world = std::shared_ptr<SceneWorld>(new SceneWorld(GameData::Flags.count("map") ? GameData::Flags.at("map") : 1));
	SceneManager::GetInstance().SetScene(world);

	PersProjInfo::GetRegularInstance().zNear = 1.f;
	PersProjInfo::GetRegularInstance().zFar = 5000.f;
	PersProjInfo::GetRegularInstance().FOV = 90.f;
	OrthoProjInfo::GetRegularInstance().zNear = 100.f;
	OrthoProjInfo::GetRegularInstance().zFar = -100.f;
	OrthoProjInfo::GetRegularInstance().changed = true;

	return true;
}

// Finds all the png files to make the texture atlas for tiles/particles
void Game::SetupTextureAtlas()
{
	std::vector<std::string> vs = Utils::GetAllFiles("res/sprites/tiles", "png");
	for(auto& x : vs)
		x = "res/sprites/tiles/" + x;

	for(auto x : Utils::GetAllFiles("res/sprites/particles", "png"))
		vs.push_back("res/sprites/particles/" + x);

	for(auto x : Utils::GetAllFiles("res/sprites/special", "png"))
		vs.push_back("res/sprites/special/" + x);


	for (std::string s : vs)
		TextureAtlas::m_textureAtlas.AddTexture(s);
	TextureAtlas::m_textureAtlas.Generate(32, false, "res/tiles.png");
	TextureAtlas::m_textureAtlas.ShortenTextureList();
	ResourceManager::GetInstance().LoadTexture("res/tiles.png");


	//// SPECIAL FILE
	//TextureAtlas::m_textureAtlas = TextureAtlas(16);
	//vs = Utils::GetAllFiles("res/sprites/special", "png");
	//for(auto& x : vs)
	//	x = "res/sprites/special/" + x;

	//for (std::string s : vs)
	//	TextureAtlas::m_textureAtlas.AddTexture(s);
	//TextureAtlas::m_textureAtlas.Generate(16, false, "res/tiles.png");
	//TextureAtlas::m_textureAtlas.ShortenTextureList();
}

void Game::renderSceneCB()
{
	HandleInput();

	if (m_exit)
	{
		
		//GameData::SaveToFile();
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
	// Exit
	if (InputManager::GetInstance().FrameKeyStatus(A_Exit, KeyStatus::KeyPressed))
		m_exit = true;

	// Draw normals
	if (InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_F1, KeyPressed))
		Globals::DEBUG_DRAW_NORMALS = !Globals::DEBUG_DRAW_NORMALS;

	// Draw outlines
	if (InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_F2, KeyStatus::KeyPressed))
		Globals::DEBUG_DRAW_TILE_OUTLINES = !Globals::DEBUG_DRAW_TILE_OUTLINES;

	// Reset lock level
	if (InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_F3, KeyStatus::KeyPressed))
	{
		InputManager::GetInstance().PopLockLevel();
		InputManager::GetInstance().PopLockLevel();
		InputManager::GetInstance().PopLockLevel();
		InputManager::GetInstance().PopLockLevel();
		InputManager::GetInstance().PopLockLevel();
		InputManager::GetInstance().SetLockLevel(0);
	}

	// Mute
//	if (InputManager::GetInstance().FrameKeyStatus(A_Mute, KeyStatus::KeyPressed))
//		MuteButton();

}

void Game::MuteButton()
{
	GameData::Options.at("mute") = !std::get<bool>(GameData::Options.at("mute"));
	SoundManager::GetInstance().SetMasterVolume(std::get<bool>(GameData::Options.at("mute")) ? 0.f : 1.f);
}

