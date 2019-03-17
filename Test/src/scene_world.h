#ifndef SCENE_WORLD_H__
#define SCENE_WORLD_H__

#include <iostream>
#include "scene.h"
#include "transform.h"
#include "entity.h"
#include "map_handler.h"
#include "eventManager.h"
#include "menu.h"
#include "physics_manager.h"
#include "event_command_line.h"

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
	void calculate_physics();

private:
	void setup_physics_manager();
	
private:
	std::shared_ptr<Entity> m_player = NULL;
	std::shared_ptr<Menu> m_menu = NULL;
	EventManager m_eventManager;
	//CollisionManager m_collisionManager;
	EventCommandLine m_event_command_line;
	physics_manager m_physics_manager;
	bool m_zoom;
	bool m_bloomEffect;
	std::shared_ptr<JsonHandler> m_jsonHandler = NULL;
	unsigned int m_fontTitle;
	unsigned int m_fontFPS;
};

#endif