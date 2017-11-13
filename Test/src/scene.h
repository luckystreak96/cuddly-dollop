#ifndef SCENE_H__
#define SCENE_H__

#include <memory>

class Scene
{
public:
	virtual bool Init() = 0;
	virtual std::shared_ptr<Scene> Act() = 0;
	virtual void Draw() = 0;
	virtual std::shared_ptr<Scene> Update() = 0;
	virtual void LoadAllResources() = 0;
	virtual void UnloadAllResources() = 0;
};

#endif // !SCENE_MANAGER_H__
