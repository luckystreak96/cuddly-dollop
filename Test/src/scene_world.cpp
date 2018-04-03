#include "scene_world.h"
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
#include "battleData.h"

SceneWorld::SceneWorld(unsigned int map_id) : m_zoom(false)
{
	m_currentMap = map_id;
	Init();
	SoundManager::GetInstance();
	m_fade.SetFade(true);
	static bool firstEverLoad = true;
	if (firstEverLoad)
		m_fade.ForceFadeValue(-0.5f);
	firstEverLoad = false;
}

void SceneWorld::Brb()
{
	NextScene = SceneGenData();
#ifdef _DEBUG
	FontManager::GetInstance().DisableFont(m_fontFPS);
#endif
}

void SceneWorld::Resume()
{
	m_fade.SetFade(true);
#ifdef _DEBUG
	FontManager::GetInstance().EnableFont(m_fontFPS);
#endif
	Scene::Resume();
}

bool SceneWorld::Init()
{
	GameData::EmplaceFlag("map", m_currentMap);

	_type = ST_World;
	NextScene = SceneGenData();
	m_jsonHandler = std::shared_ptr<JsonHandler>(new JsonHandler(m_currentMap));

	m_bloomEffect = false;

	OrthoProjInfo::GetRegularInstance().changed = true;
	//m_camera._transform = Transformation();

	m_mapHandler = std::shared_ptr<MapHandler>(new MapHandler(m_currentMap, m_jsonHandler));
	m_camera._mapsize = m_mapHandler->GetMapSize();
	m_camera.ForcePosition(m_camera.MapCenter() - Vector3f(2.0f, 2.0f, 0));
	m_camera._style = CAMSTYLE_FollowDadNoScale;
	m_collisionManager.SetMapTiles(m_mapHandler->Tiles());

	m_celist = EntityFactory::GetEntities(m_currentMap, m_jsonHandler);
	m_eventManager.SetEntitiesMap(&m_celist);
	m_collisionManager.SetEntities(&m_celist);

	if (m_celist.count(1))
	{
		m_player = m_celist.at(1);
		m_player->Graphics()->SetTexture(GameData::PlayerSprite);
		if (GameData::Loading && GameData::Positions.count("player"))
		{
			GameData::Loading = false;
			m_player->Physics()->AbsolutePosition(GameData::Positions.at("player"));
		}
	}

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

void SceneWorld::ShowStats()
{
	for (auto& x : BattleData::Party)
		FontManager::GetInstance().CreateFloatingText(m_celist.at(1)->PhysicsRaw()->PositionRef(), std::to_string(x->_Fighter->Strength.Real));
}

SceneWorld::~SceneWorld()
{
	FontManager::GetInstance().RemoveFont(m_fontFPS);
	//FontManager::GetInstance().RemoveFont(m_fontTitle);
}

void SceneWorld::ManageInput()
{
	Scene::ManageInput();

	if (InputManager::GetInstance().FrameKeyStatus('B', KeyPressed))
		m_bloomEffect = !m_bloomEffect;

	if (InputManager::GetInstance().FrameKeyStatus('T', KeyPressed))
	{
		GameData::SaveToFile();
		FontManager::GetInstance().CreateFloatingText(m_player->Physics()->Position(), "Game Saved!")->GetGraphics()->SetColorAll(Vector3f(0.1f, 0.65f, 1));
	}

	if (InputManager::GetInstance().FrameKeyStatus('Y', KeyPressed))
		ShowStats();

	//if (InputManager::GetInstance().FrameKeyStatus('Z', AnyRelease))
	//{
	//	m_zoom = !m_zoom;
	//	float value = m_zoom ? 2.0f : 1.0f;
	//	m_World->SetScale(value, value, 1);
	//	if (m_celist.count(Camera::Target))
	//		for (int i = 0; i < 30; i++)
	//			Camera::Follow(m_celist.at(Camera::Target)->Physics()->Position(), m_World.get());
	//}
}

void SceneWorld::Interact()
{
	if (InputManager::GetInstance().FrameKeyStatus(A_Accept, KeyStatus::KeyPressed) && m_player) {
		Vector3f pos = m_player->Physics()->GetCenter();
		Direction dir = m_player->Graphics()->GetDirection();

		float distance = 0.2f;
		if (dir == dir_Left || dir == dir_Right)
			distance += m_player->Physics()->GetSize().x / 2;
		else if (dir == dir_Up || dir == dir_Down)
			distance += m_player->Physics()->GetSize().y / 2;

		dir == dir_Up ? pos.y += distance : pos.y = pos.y;
		dir == dir_Down ? pos.y -= distance : pos.y = pos.y;
		dir == dir_Right ? pos.x += distance : pos.x = pos.x;
		dir == dir_Left ? pos.x -= distance : pos.x = pos.x;

		std::shared_ptr<Entity> inter = NULL;
		for (auto x : m_celist)
		{
			if (Physics::Intersect2D(x.second->Physics()->GetBoundingBox(), pos))
			{
				if (x.second == m_player)
					break;
				inter = x.second;
				break;
			}
		}

		if (inter != NULL)
		{
			inter->Graphics()->SetDirection(m_player->Graphics());
			TriggerEvents(inter->GetID());
		}
	}
}

void SceneWorld::TriggerEvents(unsigned int entity_id)
{
	if (m_celist.count(entity_id))
		for (auto x : *m_celist.at(entity_id)->GetQueues())
			if (x->GetActivationType() == AT_Interact)
				m_eventManager.PushBack(x);
}

SceneGenData SceneWorld::Update()
{
	SceneGenData next = NextScene;

	// Needs to be called here so the EventQueues can set render
	Renderer::GetInstance().Clear();
	Animation::AnimationCounter((float)ElapsedTime::GetInstance().GetElapsedTime());
	Interact();
	m_eventManager.Update(ElapsedTime::GetInstance().GetElapsedTime());

	if (next != NextScene)
		m_fade.SetFade(false);

	for (auto it : m_celist)
		it.second->Physics()->DesiredMove();

	//Collision
	std::map<unsigned int, std::shared_ptr<Entity>> collided = m_collisionManager.CalculateCollision();
	for (auto &pair : collided)
	{
		auto entity = pair.second;
		// ...push back the event
		for (auto x : *entity->GetQueues())
			if (x->GetActivationType() == AT_Touch)
				// If he wasn't just touched...
				if (!entity->_justTouched || x->GetID() != -1)
					m_eventManager.PushBack(x);

		entity->_justTouched = true;
	}

	// Handle just-touched
	for (auto &pair : m_celist)
	{
		if (!collided.count(pair.first))
			pair.second->_justTouched = false;
	}

	//Update
	for (auto it : m_celist)
		it.second->Update();

	if (GameData::Positions.count("player"))
		GameData::Positions.at("player") = m_player->Physics()->Position();
	else
		GameData::Positions.emplace("player", m_player->Physics()->Position());

	m_mapHandler->Update(OrthoProjInfo::GetRegularInstance().changed);

	SetAudioPosition();
	SoundManager::GetInstance().Update();

	if (m_celist.count(m_camera.Target))
		m_camera.SetFollow(m_celist.at(m_camera.Target)->Physics()->Position() + Vector3f(0.5f, 0.5f, 0));

	//Display FPS
#ifdef _DEBUG
	FontManager::GetInstance().SetText(m_fontFPS, /*std::to_string(m_celist.at(1)->PhysicsRaw()->PositionRef().z),*/std::to_string(ElapsedTime::GetInstance().GetFPS()),
		Vector3f(0, OrthoProjInfo::GetRegularInstance().Top * 2.f / OrthoProjInfo::GetRegularInstance().Size - 0.5f, 0));
#endif

	srand(clock());
	FontManager::GetInstance().Update(ElapsedTime::GetInstance().GetElapsedTime());

	return NextScene;
}

void SceneWorld::Draw()
{
	Scene::DrawBegin();

	// Set the renders
	m_mapHandler->SetRender();

	for (auto it : m_celist)
		it.second->SetRender();
	FontManager::GetInstance().SetRender();

	// Can enable post-processing effects here
	Scene::DrawEnd();
	// can disable post-processing effects here
}

void SceneWorld::SetAudioPosition()
{
	if (m_player != NULL)
		SoundManager::GetInstance().SetListenerPosition(m_player->Physics()->Position(), m_player->Physics()->Velocity());
	else
		SoundManager::GetInstance().SetListenerPosition();
}
