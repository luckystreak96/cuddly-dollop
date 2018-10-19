#ifndef SCENE_BATTLE_H__
#define SCENE_BATTLE_H__

#include <iostream>
#include "scene.h"
#include "transform.h"
#include "entity.h"
#include "map_handler.h"
#include "eventManager.h"
#include "battleManager.h"
#include "actor.h"

class SceneBattle : public Scene
{
public:
	SceneBattle();
	~SceneBattle();
	bool Init();
	void Draw();
	void Brb() {}
	void Resume() {}
	SceneGenData Update();
	void ManageInput();

public:
	std::shared_ptr<Scene> _prevScene;
	
private:
	EventManager m_eventManager;
	BattleManager m_battle;
	std::vector<Actor_ptr> m_actors;

	std::vector<Fighter_ptr> m_fighters;
	bool m_zoom;
	bool m_bloomEffect;
	std::shared_ptr<JsonHandler> m_jsonHandler = NULL;
	unsigned int m_fontTitle;
	unsigned int m_fontFPS;
};

#endif
