#ifndef SCENE_H__
#define SCENE_H__

#include <memory>

enum SceneType { ST_World, ST_Battle };

class Scene;

struct SceneGenData 
{
	SceneType sceneType;
	unsigned int id;
	std::shared_ptr<Scene> scene;
	bool operator!=(const SceneGenData& sgd)
	{
		if (this->id != sgd.id || this->sceneType != sgd.sceneType || this->scene != sgd.scene)
			return true;
		return false;
	}
};

class Scene
{
public:
	virtual bool Init() = 0;
	virtual void Brb() = 0;
	virtual void Resume() = 0;
	virtual SceneGenData Act() = 0;
	virtual void Draw() = 0;
	virtual SceneGenData Update() = 0;

	bool isBrb;
	//virtual void LoadAllResources() = 0;
	//virtual void UnloadAllResources() = 0;
};

#endif // !SCENE_MANAGER_H__
