#ifndef SCENE_MANAGER_H__
#define SCENE_MANAGER_H__

#include <iostream>
#include "scene.h"
#include "resource_manager.h"

class SceneManager
{
public:
	static SceneManager& GetInstance()
	{
		static SceneManager instance;
		return instance;
	}

	void SetScene(std::shared_ptr<Scene> s);
	void Act();

private:
	SceneManager() {}
	std::shared_ptr<Scene> m_currentScene;
};

#endif // !SCENE_MANAGER_H__
