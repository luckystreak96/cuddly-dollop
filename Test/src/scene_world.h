#ifndef SCENE_WORLD_H__
#define SCENE_WORLD_H__

#include <iostream>
#include "scene.h"
#include "transform.h"
#include "resource_manager.h"
#include "input_manager.h"
#include "bloom.h"
#include "graphicsComponent.h"
#include "entity.h"
#include "map_handler.h"
#include "soundManager.h"
#include "renderer.h"
#include "font.h"
#include "fontManager.h"
#include "eventManager.h"
#include "jsonHandler.h"
#include "eventFactory.h"
#include "entityFactory.h"
#include "collisionManager.h"
#include "fade.h"
#include "particleGenerator.h"

class SceneWorld : public Scene
{
public:
	SceneWorld(unsigned int map_id);
	~SceneWorld();
	bool Init();
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
	
private:
	void SetOrthoStuffs();

private:
	std::shared_ptr<Entity> m_player = NULL;
	static SceneGenData NextScene;
	std::shared_ptr<Scene> m_nextScene = NULL;
	EventManager m_eventManager;
	std::shared_ptr<MapHandler> m_mapHandler = NULL;
	std::shared_ptr<Transformation> m_World = NULL;
	std::map<unsigned int, std::shared_ptr<Entity>> m_celist;
	CollisionManager m_collisionManager;
	bool m_pause;
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
	Bloom m_bloom;
	Fade m_fade;
};

#endif