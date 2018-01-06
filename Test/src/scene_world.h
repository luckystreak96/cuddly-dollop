#ifndef SCENE_WORLD_H__
#define SCENE_WORLD_H__

#include <iostream>
#include "scene.h"
#include "transform.h"
#include "entity.h"
#include "map_handler.h"
#include "collisionManager.h"
#include "fade.h"
#include "eventManager.h"

class SceneWorld : public Scene
{
public:
	SceneWorld(unsigned int map_id);
	~SceneWorld();
	bool Init();
	void Brb();
	void Resume();
	SceneGenData Act();
	void Draw();
	SceneGenData Update();
	void Interact();
	void TriggerEvents(unsigned int entity_id);
	void LoadAllResources();
	void UnloadAllResources();
	void ManageInput();
	void SetAudioPosition();
	static void SetNextScene(SceneGenData sgd);
	void RenderPass();
	static SceneGenData NextScene;
	
private:
	void SetOrthoStuffs();

private:
	std::shared_ptr<Entity> m_player = NULL;
	std::shared_ptr<Scene> m_nextScene = NULL;
	EventManager m_eventManager;
	std::shared_ptr<MapHandler> m_mapHandler = NULL;
	std::shared_ptr<Transformation> m_World = NULL;
	std::map<unsigned int, std::shared_ptr<Entity>> m_celist;
	CollisionManager m_collisionManager;
	bool m_pause;
	bool m_zoom;
	bool m_resources_loaded;
	bool m_acceptInput;
	bool m_bloomEffect;
	bool m_drawinited;
	int m_numFrames = 0;
	std::shared_ptr<JsonHandler> m_jsonHandler = NULL;
	unsigned int m_currentMap;
	unsigned int m_fontTitle;
	unsigned int m_fontFPS;
	Vector3f m_backupTrans;
	Fade m_fade;
};

#endif