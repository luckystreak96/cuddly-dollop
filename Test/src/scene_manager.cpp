#include "scene_manager.h"
#include "scene_world.h"

void SceneManager::SetScene(std::shared_ptr<Scene> s)
{
	m_currentScene = s;
}

void SceneManager::Act()
{
	std::shared_ptr<Scene> temp = m_currentScene;
	SceneGenData temp2 = m_currentScene->Act();
	if (temp2.id != 0)
		m_currentScene = CreateScene(temp2);

	//Scenes have changed
	if (temp != m_currentScene)
	{
		//temp->UnloadAllResources();
		//ResourceManager::GetInstance().UnloadAllResources();
	}
}

std::shared_ptr<Scene> SceneManager::CreateScene(SceneGenData sgd)
{
	switch (sgd.sceneType)
	{
	case ST_World:
		return std::shared_ptr<Scene>(new SceneWorld(sgd.id));
	}
}
