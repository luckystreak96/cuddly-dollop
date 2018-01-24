#ifndef SCENE_H__
#define SCENE_H__

#include <memory>
#include <map>
#include "fade.h"

enum SceneType { ST_World, ST_Battle };

class Scene;
class MapHandler;
class Transformation;
class Vector3f;
class Entity;

struct SceneGenData 
{
	SceneGenData() { sceneType = ST_World; id = 0; scene = NULL; }
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
	virtual void Brb();
	virtual void Resume();
	virtual SceneGenData Act();
	virtual void Draw() = 0;
	virtual void DrawBegin();
	virtual void DrawEnd();
	virtual SceneGenData Update() = 0;
	virtual void ManageInput();
	virtual void SetAudioPosition();

protected:
	void SetOrthoStuffs();

public:
	static SceneGenData NextScene;
	static bool _pause;
	static int _numFrames;
	bool isBrb;
	SceneType _type;

protected:
	std::shared_ptr<MapHandler> m_mapHandler = NULL;
	std::shared_ptr<Transformation> m_World = NULL;
	std::shared_ptr<Scene> m_nextScene = NULL;
	Fade m_fade;
	unsigned int m_currentMap;
	Vector3f m_backupTrans;
	std::map<unsigned int, std::shared_ptr<Entity>> m_celist;
};

#endif // !SCENE_MANAGER_H__
