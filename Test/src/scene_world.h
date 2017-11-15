#ifndef SCENE_WORLD_H__
#define SCENE_WORLD_H__

#include <iostream>
#include "scene.h"
#include "shadowMapFBO.h"
#include "transform.h"
#include "array2d.h"
#include "elapsedTime.h"
#include "glutBackend.h"
#include "physics_2d.h"
#include "resource_manager.h"
#include "input_manager.h"
#include <gl/freeglut.h>
#include "basicEffect.h"
#include "bloomEffect.h"
#include "combineEffect.h"
#include "transparencyEffect.h"
#include "animation.h"
#include "define_gl.h"
#include "bloom.h"
#include "trail.h"
#include "graphicsComponent.h"
#include "entity.h"
#include "map_handler.h"
#include "soundManager.h"
#include "heightFog.h"
#include "renderer.h"
#include "font.h"
#include "fontManager.h"
#include "eventManager.h"
#include "jsonHandler.h"
#include "eventFactory.h"
#include "entityFactory.h"

class SceneWorld : public Scene
{
public:
	SceneWorld(unsigned int map_id);
	~SceneWorld();
	bool Init();
	std::shared_ptr<Scene> Act();
	void Draw();
	std::shared_ptr<Scene> Update();
	void Interact();
	void TriggerEvents(unsigned int entity_id);
	void LoadAllResources();
	void UnloadAllResources();
	void ManageInput();
	void SetAudioPosition();
	static void SetNextScene(std::shared_ptr<Scene> s);
	
public:
	void RenderPass();

private:
	std::shared_ptr<Entity> m_player = NULL;
	std::shared_ptr<Camera> m_camera;
	static std::shared_ptr<Scene> NextScene;
	EventManager m_eventManager;
	//Map* m_map = NULL;
	std::shared_ptr<MapHandler> m_mapHandler;
	std::shared_ptr<Transformation> m_World;
	std::map<unsigned int, std::shared_ptr<Entity>> m_celist;
	bool m_pause;
	bool m_resources_loaded;
	bool m_acceptInput;
	bool m_bloomEffect;
	bool m_drawinited;
	int m_numFrames = 0;
	std::shared_ptr<JsonHandler> m_jsonHandler;
	unsigned int m_currentMap;
	unsigned int m_fontTitle;
	unsigned int m_fontFPS;
	Trail m_trail;
	Bloom m_bloom;
	HeightFog m_fog;
	const std::string DATA_FILE = "res/data/001.json";
};

#endif