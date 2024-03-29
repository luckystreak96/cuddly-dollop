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
#include "menuSettings.h"
#include "physics_rpg.h"

#include <algorithm>

SceneWorld::SceneWorld(unsigned int map_id) : m_zoom(false), m_physics_manager(new physics_rpg())
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
//#ifdef _DEBUG
	FontManager::GetInstance().DisableFont(m_fontFPS);
//#endif
}

void SceneWorld::Resume()
{
	m_fade.SetFade(true);
//#ifdef _DEBUG
	FontManager::GetInstance().EnableFont(m_fontFPS);
//#endif
	Scene::Resume();
}

bool SceneWorld::Init()
{
	GameData::EmplaceFlag("map", m_currentMap);

	_type = ST_World;
	NextScene = SceneGenData();
	m_jsonHandler = std::shared_ptr<JsonHandler>(new JsonHandler(m_currentMap));

	OrthoProjInfo::GetRegularInstance().changed = true;
	//m_camera._transform = Transformation();

	m_mapHandler = std::shared_ptr<MapHandler>(new MapHandler(m_currentMap, m_jsonHandler));
	m_camera._mapsize = m_mapHandler->GetMapSize();
	m_camera.ForcePosition(m_camera.MapCenter() - Vector3f(2.0f, 2.0f, 0));
	m_camera._style = CAMSTYLE_FollowDadNoScale;
	//m_collisionManager.SetMapTiles(m_mapHandler->OrderedTiles());

	m_celist = EntityFactory::GetEntities(m_currentMap, m_jsonHandler);
	m_camera.ForcePosition(m_celist.at(1)->PhysicsRaw()->get_position_ref());
	m_eventManager.SetEntitiesMap(&m_celist);
	//m_collisionManager.SetEntities(&m_celist);

	if (m_celist.count(1))
	{
		m_player = m_celist.at(1);
        m_player->Graphics()->set_texture(GameData::PlayerSprite);
		if (GameData::Loading && GameData::Positions.count("player"))
		{
			GameData::Loading = false;
			m_player->Physics()->AbsolutePosition(GameData::Positions.at("player"));
		}
	}

//#ifdef _DEBUG
	//m_fontTitle = FontManager::GetInstance().AddFont(false, false);
	m_fontFPS = FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(m_fontFPS, 0.5f, 0.5f);
//#endif

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

	setup_physics_manager();

	m_event_command_line = EventCommandLine(&m_eventManager, m_mapHandler.get());

	return true;
}

void SceneWorld::setup_physics_manager()
{
	for (auto& x : m_celist)
		m_physics_manager.add_physics(x.second->PhysicsRaw());

	for (auto& x : *m_mapHandler->Tiles())
		m_physics_manager.add_physics(x->PhysicsRaw());
}


void SceneWorld::ShowStats()
{
	for (auto& x : BattleData::Party)
		FontManager::GetInstance().CreateFloatingText(m_celist.at(1)->PhysicsRaw()->get_position_ref(), std::to_string(x->Strength.Real));
}

SceneWorld::~SceneWorld()
{
	FontManager::GetInstance().RemoveFont(m_fontFPS);
	//FontManager::GetInstance().RemoveFont(m_fontTitle);
}

void SceneWorld::ManageInput()
{
	Scene::ManageInput();

	bool input_over = false;
	std::vector<unsigned int> input = InputManager::GetInstance().FrameTypingKeys();
	for(auto x : input)
		input_over = m_event_command_line.handle_input((char)x) || input_over;

	if(m_event_command_line.is_reading()) {
		_pause = true;
		return;
	}
	else if (_pause && input_over)
	{
		_pause = false;
		return;
	}

	if (InputManager::GetInstance().FrameKeyStatus('[', KeyPressed))
		Camera::_currentCam->SetScale(Vector3f(4.0f, 4.0f, 1.0f));
	if (InputManager::GetInstance().FrameKeyStatus(']', KeyPressed))
		Camera::_currentCam->SetScale(Vector3f(1.0f, 1.0f, 1.0f));
	if (InputManager::GetInstance().FrameKeyStatus('=', KeyPressed))
		Camera::_currentCam->SetScale(Vector3f(0.1f, 0.1f, 1.0f));
	if (InputManager::GetInstance().FrameKeyStatus(';', KeyPressed))
		FontManager::GetInstance().CreateFloatingText(m_celist.at(1)->PhysicsRaw()->get_position_ref(), std::to_string(
				m_celist.at(1)->PhysicsRaw()->get_position_ref().x));
	// Mute
	if (InputManager::GetInstance().FrameKeyStatus(A_Mute, KeyStatus::Release))
	    SoundManager::GetInstance().toggle_BGM_mute();

	if (InputManager::GetInstance().FrameKeyStatus('T', KeyPressed))
	{
		// Make sure GameData knows the player position
		if (GameData::Positions.count("player"))
			GameData::Positions.at("player") = m_player->PhysicsRaw()->get_position_ref();
		else
			GameData::Positions.emplace("player", m_player->PhysicsRaw()->get_position_ref());

		GameData::SaveToFile();
		FontManager::GetInstance().CreateFloatingText(m_player->Physics()->get_position(), "Game Saved!")->GetGraphics()->SetColorAll(Vector3f(0.1f, 0.65f, 1));
	}

	if (InputManager::GetInstance().FrameKeyStatus('Y', KeyPressed))
		ShowStats();

	if (InputManager::GetInstance().FrameKeyStatus(A_Menu, KeyPressed, 2U))
	{
		if (!m_menu)
			m_menu = std::shared_ptr<Menu>(new Menu());

		if (m_menu->_done)
		{
			m_menu->Open(std::shared_ptr<MenuSettings>(new MenuSettings()));
			m_camera._paused = true;
		}
		else
			m_menu->Close();
	}

	//if (InputManager::GetInstance().FrameKeyStatus('Z', AnyRelease))
	//{
	//	m_zoom = !m_zoom;
	//	float value = m_zoom ? 2.0f : 1.0f;
	//	m_World->SetScale(value, value, 1);
	//	if (m_celist.count(Camera::Target))
	//		for (int i = 0; i < 30; i++)
	//			Camera::Follow(m_celist.at(Camera::Target)->Physics()->get_position(), m_World.get());
	//}
}

void SceneWorld::Interact()
{
	if (InputManager::GetInstance().FrameKeyStatus(A_Accept, KeyStatus::KeyPressed) && m_player) {
		Vector3f pos = m_player->Physics()->GetCenter();
		pos.z = m_player->PhysicsRaw()->get_position_ref().z;

		Direction dir = dir_Down;
		PlayerGraphicsComponent* pgc = dynamic_cast<PlayerGraphicsComponent*>(m_player->Graphics().get());
		if(pgc)
			dir = pgc->GetDirection();

		float distance = 0.2f * 64.0f;
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
			if(inter->Graphics())
			{
				pgc = dynamic_cast<PlayerGraphicsComponent*>(inter->Graphics().get());
				if(pgc)
					pgc->SetDirection(m_player->Graphics().get());
			}
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

	if (m_menu && !m_menu->_done)
		m_menu->Update();

	if (!m_menu || m_menu->_done)
	{
		// Needs to be called here so the EventQueues can set render
		Renderer::GetInstance().Clear();
		Animation::AnimationCounter((float)ElapsedTime::GetInstance().GetElapsedTime());
		Interact();
		m_eventManager.Update(ElapsedTime::GetInstance().GetElapsedTime());

		if (next != NextScene)
		{
			m_fade.SetFade(false);
			SoundManager::GetInstance().SetBGMFadeout();
		}

		for (auto it : m_celist)
			it.second->Physics()->DesiredMove();

		calculate_physics();

		//Update
		for (auto it : m_celist)
			it.second->Update();

		m_mapHandler->Update(OrthoProjInfo::GetRegularInstance().changed);

		if (m_celist.count(m_camera.Target))
			m_camera.SetFollow(m_celist.at(m_camera.Target)->Physics()->get_position() + Vector3f(0.5f, 0.5f, 0));

		SetAudioPosition();
		SoundManager::GetInstance().Update();

		UpdateHUD();
	}

	return NextScene;
}

void SceneWorld::calculate_physics()
{
	// Replace collision
	m_physics_manager.calculate_frame();

	for (auto& candidate : m_celist)
	{
		if (candidate.second->PhysicsRaw()->_collided_last_frame > -1)
		{
			std::shared_ptr<Entity> ent = m_celist.at(candidate.second->PhysicsRaw()->_collided_last_frame);
			// ...push back the event
			for (auto queue : *ent->GetQueues())
				if (queue->GetActivationType() == AT_Touch)
					// If he wasn't just touched...
					if (!ent->_justTouched || queue->GetID() != -1)
						m_eventManager.PushBack(queue);

			ent->_justTouched = true;

			candidate.second->PhysicsRaw()->_collided_last_frame = -1;
		}
		else
			candidate.second->_justTouched = false;
	}
}


void SceneWorld::UpdateHUD()
{
	//Display FPS
//#ifdef _DEBUG
	//static double fps = 0;
	//unsigned int curfps = ElapsedTime::GetInstance().GetFPS();
	if (ElapsedTime::GetInstance().OneSecond())
	{
      FontManager::GetInstance().SetText(m_fontFPS, std::to_string(ElapsedTime::GetInstance().GetFPS()),
			Vector3f(0, OrthoProjInfo::GetRegularInstance().Top * 2.f - 32.0f, -10));
      //std::cout << ElapsedTime::GetInstance().GetFPS() << std::endl;
	}
	//std::cout << m_celist.at(1)->PhysicsRaw()->GetBoundingBox()[0] << std::endl;
//#endif

	//srand(clock());

	FontManager::GetInstance().Update(ElapsedTime::GetInstance().GetElapsedTime());
}

void SceneWorld::Draw()
{
	Scene::DrawBegin();

	// Set the renders
	m_mapHandler->SetRender();

	if (m_menu && !m_menu->_done)
		m_menu->SetRender();

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
		SoundManager::GetInstance().SetListenerPosition(m_player->Physics()->get_position(),
                                                        m_player->Physics()->get_velocity());
	else
		SoundManager::GetInstance().SetListenerPosition();
}
