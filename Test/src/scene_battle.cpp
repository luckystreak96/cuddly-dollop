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

SceneGenData SceneBattle::NextScene = SceneGenData();

SceneBattle::SceneBattle() : m_acceptInput(false), m_currentMap(3), m_drawinited(false), m_zoom(false),
m_battle(_actors)
{
	_actors.push_back(Actor_ptr(ActorFactory::BuildBaseAlly()));
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
	NextScene = SceneGenData();
	m_jsonHandler = std::shared_ptr<JsonHandler>(new JsonHandler(m_currentMap));

	m_bloomEffect = false;

	OrthoProjInfo::GetRegularInstance().changed = true;
	m_World = std::shared_ptr<Transformation>(new Transformation());

	m_pause = false;
	m_acceptInput = true;

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
		m_party.at(i)->SetPhysics(Vector3f(3.0f, 4.0f + i, 4.0f), Vector3f());

	for (int i = 0; i < m_enemies.size(); i++)
		m_enemies.at(i)->SetPhysics(Vector3f(12.0f, 4.0f + i, 4.0f), Vector3f());

	m_eventManager.SetEntitiesMap(&m_celist);

#ifdef _DEBUG
	//m_fontTitle = FontManager::GetInstance().AddFont(false, false);
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
	//FontManager::GetInstance().RemoveFont(m_fontTitle);
}

void SceneBattle::ManageInput()
{
	InputManager::GetInstance().SetupFrameKeys();

	if (InputManager::GetInstance().FrameKeyStatus('B', AnyRelease))
		m_bloomEffect = !m_bloomEffect;

	if (InputManager::GetInstance().FrameKeyStatus('F', AnyRelease))
		m_fade.SetFade(true);
	if (InputManager::GetInstance().FrameKeyStatus('G', AnyRelease))
		m_fade.SetFade(false);

	if (InputManager::GetInstance().FrameKeyStatus('P', AnyRelease))
		m_pause = !m_pause;

	if (InputManager::GetInstance().FrameKeyStatus('Z', AnyRelease))
	{
		m_zoom = !m_zoom;
		float value = m_zoom ? 2.0f : 1.0f;
		m_World->SetScale(value, value, 1);
		if (m_celist.count(Camera::Target))
			for (int i = 0; i < 30; i++)
				Camera::Follow(m_celist.at(Camera::Target)->Physics()->Position(), m_World.get());
	}
}

SceneGenData SceneBattle::Act()
{
	SceneGenData result;
	result.id = 0;
	result.sceneType = ST_World;
	ManageInput();

	//RENDER SETUP WITH FRAME BY FRAME
	if (!m_pause || m_numFrames > 0)
	{
		//If we pause to slowly pass frames...
		if (m_numFrames > 0)
		{
			m_numFrames--;
			//...then set the elapsedtime to the desired amount (in fps)
			ElapsedTime::GetInstance().SetBufferElapsedTime(60.f);
		}
		result = Update();
	}

	//DRAW
	Draw();

	if (!m_fade.IsDone() || result.id == 0 || result.scene == NULL)
	{
		result.id = 0;
		result.scene = NULL;
		return result;
	}
	result.scene->Resume();
	return result;
}

void SceneBattle::Draw()
{
	RenderPass();
}

// Handle input?
void SceneBattle::Interact()
{
}

SceneGenData SceneBattle::Update()
{
	SceneGenData next = NextScene;

	// Needs to be called here so the EventQueues can set render
	Renderer::GetInstance().Clear();
	Animation::AnimationCounter((float)ElapsedTime::GetInstance().GetElapsedTime());
	//Interact();
	m_eventManager.Update(ElapsedTime::GetInstance().GetElapsedTime());

	if (m_fade.IsDone())
		m_battle.Update();

	if (m_battle._done)
	{
		NextScene.scene = _prevScene;
		NextScene.sceneType = ST_World;
		NextScene.id = 1;
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

	m_mapHandler->Update();

	SetAudioPosition();
	SoundManager::GetInstance().Update();

	if (m_celist.count(Camera::Target))
		Camera::Follow(m_celist.at(Camera::Target)->Physics()->Position(), m_World.get());

	//Display FPS
#ifdef _DEBUG
	FontManager::GetInstance().SetText(m_fontFPS, std::to_string(ElapsedTime::GetInstance().GetFPS()), Vector3f(0, OrthoProjInfo::GetRegularInstance().Top / 32.0f - 0.5f, 0));
#endif

	srand(clock());
	FontManager::GetInstance().Update(ElapsedTime::GetInstance().GetElapsedTime());

	return NextScene;
}

void SceneBattle::RenderPass()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glStencilMask(0xFF);
	glClearStencil(0x00);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);

	SetOrthoStuffs();

	EffectManager::GetInstance().SetWorldTrans(&m_World->GetWOTrans().m[0][0], &m_World->GetWOTransNoTranslate().m[0][0]);
	EffectManager::GetInstance().ResetWorldUpdateFlag();

	EffectManager::GetInstance().Enable(E_Basic);

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

	if (!m_fade.IsDone())
		m_fade.Begin();

	// BLOOM
	if (m_bloomEffect)
		Bloom::GetInstance().Begin();

	// DRAW
	Renderer::GetInstance().Draw();

	// BLOOM END
	if (m_bloomEffect)
		Bloom::GetInstance().End(false);

	m_World->SetTranslation(m_backupTrans);
}

void SceneBattle::SetAudioPosition()
{
	SoundManager::GetInstance().SetListenerPosition();
}

void SceneBattle::SetNextScene(SceneGenData sgd)
{
	NextScene = sgd;
}

void SceneBattle::SetOrthoStuffs()
{
	if (OrthoProjInfo::GetRegularInstance().changed)
	{
		OrthoProjInfo* o = &OrthoProjInfo::GetRegularInstance();
		m_World->SetOrthoProj(o);
		m_World->SetTranslation(-o->Right / o->Size, -o->Top / o->Size, 0);

		EffectManager::GetInstance().SetAllTilePositions(OrthoProjInfo::GetRegularInstance().Size);

		if (m_celist.count(Camera::Target))
			for (int i = 0; i < 30; i++)
				Camera::Follow(m_celist.at(Camera::Target)->Physics()->Position(), m_World.get());

		OrthoProjInfo::GetRegularInstance().changed = false;
	}

	float value = m_zoom ? 2.0f : 1.0f;
	m_World->SetScale(value, value, 1);

	m_backupTrans = m_World->GetTranslation();

	Vector3f temp = m_backupTrans;
	temp.x *= OrthoProjInfo::GetRegularInstance().Size;
	temp.y *= OrthoProjInfo::GetRegularInstance().Size;

	// Floor so we dont move the camera half a pixel and fuck up the graphics
	temp.x = floorf(temp.x);
	temp.y = floorf(temp.y);
	m_World->SetTranslation(temp);
}
