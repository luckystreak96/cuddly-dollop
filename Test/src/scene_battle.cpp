#include "scene_battle.h"
#include "gameData.h"
#include "basicEffect.h"
#include "elapsedTime.h"
#include "singleColorEffect.h"
#include "effectManager.h"
#include "eventFactory.h"
#include "resource_manager.h"
#include "soundManager.h"
#include "renderer.h"
#include "fontManager.h"
#include "bloom.h"
#include "input_manager.h"
#include "entityFactory.h"
#include "actorFactory.h"

SceneBattle::SceneBattle() : m_zoom(false)
{
	m_currentMap = 3;
	//_actors.push_back(Actor_ptr(ActorFactory::BuildBaseAlly()));
	for (auto x : GameData::Party)
	{
		_actors.push_back(x);
		_actors.push_back(Actor_ptr(ActorFactory::BuildBaseAlly()));
	}
	_actors.push_back(Actor_ptr(ActorFactory::BuildBaseEnemy()));
	_actors.push_back(Actor_ptr(ActorFactory::BuildBaseEnemy()));
	_actors.push_back(Actor_ptr(ActorFactory::BuildBaseEnemy()));
	_actors.push_back(Actor_ptr(ActorFactory::BuildBaseEnemy()));
	m_battle = BattleManager(_actors);
	Init();
	SoundManager::GetInstance();
	m_fade.SetFade(true);
	static bool firstEverLoad = true;
	if (firstEverLoad)
		m_fade.ForceFadeValue(-0.5f);
	firstEverLoad = false;
}

bool SceneBattle::Init()
{
	_type = ST_Battle;
	NextScene = SceneGenData();
	m_jsonHandler = std::shared_ptr<JsonHandler>(new JsonHandler(m_currentMap));

	m_bloomEffect = false;

	OrthoProjInfo::GetRegularInstance().changed = true;
	m_World = std::shared_ptr<Transformation>(new Transformation());

	m_mapHandler = std::shared_ptr<MapHandler>(new MapHandler(m_currentMap, m_jsonHandler));
	Camera::Mapsize = m_mapHandler->GetMapSize();

	for (auto x : _actors)
	{
		if (x->Team == 0)
			m_party.push_back(x);
		else
			m_enemies.push_back(x);
	}

	for (int i = 0; i < m_party.size(); i++)
		m_party.at(i)->SetPhysics(Vector3f(4.0f, 3.0f + i, 4.0f), Vector3f());

	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies.at(i)->SetPhysics(Vector3f(13.0f, 3.0f + i, 4.0f), Vector3f());
		m_enemies.at(i)->GetModelMat()->SetScale(-1, 1, 1);
	}

	m_eventManager.SetEntitiesMap(&m_celist);

#ifdef _DEBUG
	m_fontFPS = FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(m_fontFPS, 0.5f, 0.5f);
#endif

	// Autorun events
	for (auto e : m_celist)
	{
		for (auto x : EventFactory::LoadEvent(m_currentMap, e.first, m_jsonHandler, m_mapHandler.get()))
		{
			e.second->AddEventQueue(x);
			if (x->GetActivationType() == AT_Autorun)
			{
				m_eventManager.PushBack(x);
			}
		}
	}

	return true;
}

SceneBattle::~SceneBattle()
{
	FontManager::GetInstance().RemoveFont(m_fontFPS);
}

void SceneBattle::ManageInput()
{
	Scene::ManageInput();

	if (InputManager::GetInstance().FrameKeyStatus('Z', AnyRelease, 5))
	{
		m_zoom = !m_zoom;
		float value = m_zoom ? 2.0f : 1.0f;
		m_World->SetScale(value, value, 1);
	}
}

SceneGenData SceneBattle::Update()
{
	SceneGenData next = NextScene;

	// Needs to be called here so the EventQueues can set render
	Renderer::GetInstance().Clear();
	Animation::AnimationCounter((float)ElapsedTime::GetInstance().GetElapsedTime());
	m_eventManager.Update(ElapsedTime::GetInstance().GetElapsedTime());

	if (m_fade.IsDone())
		m_battle.Update();

	if (m_battle._done && m_battle._winner == 0)
	{
		NextScene.scene = _prevScene;
		NextScene.sceneType = ST_World;
		NextScene.id = 1;
	}
	else if (m_battle._done && m_battle._winner != 0 && !GameData::Loading)
	{
		GameData::LoadGameData();
		NextScene.scene = NULL;
		NextScene.sceneType = ST_World;
		NextScene.id = GameData::Flags.at("map");
	}

	if (next != NextScene)
		m_fade.SetFade(false);

	for (auto it : m_celist)
		it.second->Physics()->DesiredMove();

	//Update
	for (auto it : m_celist)
		it.second->Update();

	for (auto a : _actors)
		a->Update();

	m_mapHandler->Update(OrthoProjInfo::GetRegularInstance().changed);

	SetAudioPosition();
	SoundManager::GetInstance().Update();

	Camera::MapCenter(m_World.get());

	//Display FPS
#ifdef _DEBUG
	FontManager::GetInstance().SetText(m_fontFPS, std::to_string(ElapsedTime::GetInstance().GetFPS()), Vector3f(0, OrthoProjInfo::GetRegularInstance().Top / 32.0f - 0.5f, 0));
#endif

	srand(clock());
	FontManager::GetInstance().Update(ElapsedTime::GetInstance().GetElapsedTime());

	return NextScene;
}

void SceneBattle::Draw()
{
	Scene::DrawBegin();

	// Set the renders
	m_mapHandler->SetRender();

	for (auto it : m_celist)
		it.second->SetRender();
	FontManager::GetInstance().SetRender();

	for (auto a : m_party)
		Renderer::GetInstance().Add(a);

	for (auto a : m_enemies)
		if (!a->Dead)
			Renderer::GetInstance().Add(a);

	m_battle.SetRender();

	// Can enable post-processing effects here
	Scene::DrawEnd();
	// can disable post-processing effects here
}
