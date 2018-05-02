#ifndef SCENE_WORLD_H__
#define SCENE_WORLD_H__

#include <iostream>
#include "scene.h"
#include "transform.h"
#include "entity.h"
#include "map_handler.h"
#include "collisionManager.h"
#include "eventManager.h"
#include "menu.h"

class SceneWorld : public Scene
{
public:
	SceneWorld(unsigned int map_id);
	~SceneWorld();
	bool Init();
	void Brb();
	void Resume();
	void Draw();
	void ShowStats();
	SceneGenData Update();
	void UpdateHUD();
	void Interact();
	void TriggerEvents(unsigned int entity_id);
	void ManageInput();
	void SetAudioPosition();
	
private:
	std::shared_ptr<Entity> m_player = NULL;
	std::shared_ptr<Menu> m_menu = NULL;
	EventManager m_eventManager;
	CollisionManager m_collisionManager;
	bool m_zoom;
	bool m_bloomEffect;
	std::shared_ptr<JsonHandler> m_jsonHandler = NULL;
	unsigned int m_fontTitle;
	unsigned int m_fontFPS;
};

#endif