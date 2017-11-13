#include "scene_manager.h"

void SceneManager::SetScene(std::shared_ptr<Scene> s)
{
	m_currentScene = s;
}

void SceneManager::Act()
{
	std::shared_ptr<Scene> temp = m_currentScene;
	std::shared_ptr<Scene> temp2 = m_currentScene->Act();
	if (temp2.get() != NULL)
		m_currentScene = temp2;

	//Scenes have changed
	if (temp != m_currentScene)
	{
		//temp->UnloadAllResources();
		//ResourceManager::GetInstance().UnloadAllResources();
	}
}
