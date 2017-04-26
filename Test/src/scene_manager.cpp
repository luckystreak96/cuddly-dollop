#include "scene_manager.h"

void SceneManager::SetScene(Scene* s)
{
	m_currentScene = s;
}

void SceneManager::Act()
{
	Scene* temp = m_currentScene;
	m_currentScene = m_currentScene->Act();

	//Scenes have changed
	if (temp != m_currentScene)
	{
		temp->UnloadAllResources();
		ResourceManager::GetInstance().UnloadAllResources();
	}
}
