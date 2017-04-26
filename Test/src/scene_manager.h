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

	void SetScene(Scene* s);
	void Act();

private:
	SceneManager() {}
	Scene* m_currentScene;
};

#endif // !SCENE_MANAGER_H__
