#ifndef SCENE_BATTLE_H__
#define SCENE_BATTLE_H__

#include <iostream>
#include "scene.h"
#include "transform.h"
#include "entity.h"
#include "map_handler.h"
#include "collisionManager.h"
#include "fade.h"
#include "eventManager.h"
#include "battleManager.h"

class SceneBattle : public Scene
{
public:
	SceneBattle();
	~SceneBattle();
	bool Init();
	SceneGenData Act();
	void Draw();
	SceneGenData Update();
	void Interact();
	void ManageInput();
	void SetAudioPosition();
	static void SetNextScene(SceneGenData sgd);
	void RenderPass();

public:
	std::vector<std::shared_ptr<Actor>> _actors;
	
private:
	void SetOrthoStuffs();

private:
	static SceneGenData NextScene;
	std::shared_ptr<Scene> m_nextScene = NULL;
	EventManager m_eventManager;
	BattleManager m_battle;
	std::shared_ptr<MapHandler> m_mapHandler = NULL;
	std::shared_ptr<Transformation> m_World = NULL;
	std::vector<std::shared_ptr<PlayerGraphicsComponent>> m_party;
	std::vector<std::shared_ptr<PlayerGraphicsComponent>> m_enemies;
	std::map<unsigned int, std::shared_ptr<Entity>> m_celist;
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
