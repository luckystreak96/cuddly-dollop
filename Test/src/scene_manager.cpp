#include "scene_manager.h"
#include "scene_world.h"
#include "scene_battle.h"
#include <string>
#include <iostream>

void SceneManager::SetScene(std::shared_ptr<Scene> s)
{
	m_currentScene = s;
}

void SceneManager::Act()
{
	std::shared_ptr<Scene> temp = m_currentScene;
	SceneGenData sgd = m_currentScene->Act();

	// If its a battle, remember the last sceneworld
	if (sgd.sceneType == ST_Battle)
		sgd.scene = m_currentScene;

	// If the next scene is ready, create it
	if (sgd.id != 0/* || sgd.scene != NULL*/)
		m_currentScene = CreateScene(sgd);

	//Scenes have changed
	if (temp != m_currentScene)
	{
		//temp->UnloadAllResources();
		//ResourceManager::GetInstance().UnloadAllResources();
	}
}

std::shared_ptr<Scene> SceneManager::CreateScene(SceneGenData sgd)
{
	std::shared_ptr<Scene> result;
	switch (sgd.sceneType)
	{
	case ST_World:
		if (sgd.scene != NULL)
			return sgd.scene;
		return std::shared_ptr<Scene>(new SceneWorld(sgd.id));
	case ST_Battle:
		result = std::shared_ptr<Scene>(new SceneBattle());
		dynamic_cast<SceneBattle*>(result.get())->_prevScene = sgd.scene;
		return result;
	default:
		std::cout << "No map specified for change! Error! Loading map 1!" << std::endl;
		std::string wait;
		std::getline(std::cin, wait);
		return std::shared_ptr<Scene>(new SceneWorld(1));
	}
}
