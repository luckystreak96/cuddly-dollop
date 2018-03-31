#include "scene_battle.h"

#include "gameData.h"
#include "battleData.h"
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
#include "particleManager.h"

SceneBattle::SceneBattle() : m_zoom(false)
{
	m_currentMap = 3;
	//_actors.push_back(Actor_ptr(ActorFactory::BuildBaseAlly()));
	for (auto x : BattleData::Party)
	{
		_actors.push_back(x);
		_actors.push_back(Actor_ptr(ActorFactory::BuildBaseAlly()));
		_actors.push_back(Actor_ptr(ActorFactory::BuildBaseAlly()));
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

	m_mapHandler = std::shared_ptr<MapHandler>(new MapHandler(m_currentMap, m_jsonHandler));
	m_camera._mapsize = m_mapHandler->GetMapSize();
	m_camera._lerper.MaxVelocity = 0.1f;

	for (auto x : _actors)
	{
		if (x->_Fighter->Team == 0)
			m_party.push_back(x);
		else
			m_enemies.push_back(x);
	}

	for (int i = 0; i < m_party.size(); i++)
	{
		Vector3f position = Vector3f(3.5f + i * 0.25f, 2.5f + i * 1.25f, 4.0f);
		m_party.at(i)->_Graphics->SetPhysics(position, Vector3f());
		m_party.at(i)->BasePosition = position;
		m_party.at(i)->_Graphics->_row = AE_Right; // make player face right cuz girl looks bad looking down
		m_party.at(i)->_Graphics->_animation = AE_Right; // make player face right cuz girl looks bad looking down
	}

	for (int i = 0; i < m_enemies.size(); i++)
	{
		Vector3f position = Vector3f(13.5f - i * 0.25f, 2.5f + i * 1.25f, 4.0f);
		m_enemies.at(i)->_Graphics->SetPhysics(position, Vector3f());
		m_enemies.at(i)->BasePosition = position;
		// Set to right cause the sprites are flipped
		m_enemies.at(i)->_Graphics->_row = AE_Right;
		m_enemies.at(i)->_Graphics->_animation = AE_Right;
		m_enemies.at(i)->_Graphics->GetModelMat()->SetScale(-1, 1, 1);
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

	Scene::DrawBegin();

	return true;
}

SceneBattle::~SceneBattle()
{
	FontManager::GetInstance().RemoveFont(m_fontFPS);
	m_battle._hud.Destroy();
}

void SceneBattle::ManageInput()
{
	Scene::ManageInput();

	//if (InputManager::GetInstance().FrameKeyStatus('Z', AnyRelease, 5))
	//{
	//	m_zoom = !m_zoom;
	//	float value = m_zoom ? 2.0f : 1.0f;
	//	m_World->SetScale(value, value, 1);
	//}
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
	else
		m_battle._hud.Update();

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

	static int counter = 0;
	counter++;
	//m_camera._scale = Vector3f(sinf(counter * 0.01f) / 2 + 1, sinf(counter * 0.01) / 2 + 1, 1);
	//m_camera.MapCenter();

	//Display FPS
#ifdef _DEBUG
	FontManager::GetInstance().SetText(m_fontFPS, std::to_string(ElapsedTime::GetInstance().GetFPS()), 
		Vector3f(0, OrthoProjInfo::GetRegularInstance().Top * 2.f / OrthoProjInfo::GetRegularInstance().Size - 0.5f, 0));
#endif

	srand(clock());
	FontManager::GetInstance().Update(ElapsedTime::GetInstance().GetElapsedTime());
	ParticleManager::GetInstance().Update(ElapsedTime::GetInstance().GetElapsedTime());

	return NextScene;
}

void SceneBattle::Draw()
{
	Scene::DrawBegin();

	// Set the renders
	m_mapHandler->SetRender();

	for (auto& it : m_celist)
		it.second->SetRender();
	FontManager::GetInstance().SetRender();
	ParticleManager::GetInstance().SetRender();

	for (auto& a : _actors)
		Renderer::GetInstance().Add(a->_Graphics);

	m_battle.SetRender();

	// Can enable post-processing effects here
	Scene::DrawEnd();
	// can disable post-processing effects here
}
