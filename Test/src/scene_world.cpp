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

SceneGenData SceneWorld::NextScene = SceneGenData();

SceneWorld::SceneWorld(unsigned int map_id) : m_acceptInput(false), m_currentMap(map_id), m_drawinited(false), m_zoom(false)
{
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
	FontManager::GetInstance().DisableFont(m_fontFPS);
}

void SceneWorld::Resume()
{
	FontManager::GetInstance().EnableFont(m_fontFPS);
	Camera::Mapsize = m_mapHandler->GetMapSize();
}

bool SceneWorld::Init()
{
	GameData::EmplaceFlag("map", m_currentMap);

	NextScene = SceneGenData();
	m_jsonHandler = std::shared_ptr<JsonHandler>(new JsonHandler(m_currentMap));

	m_bloomEffect = false;

	OrthoProjInfo::GetRegularInstance().changed = true;
	m_World = std::shared_ptr<Transformation>(new Transformation());

	//BasicEffect::GetInstance();

	m_pause = false;
	m_acceptInput = true;

	m_mapHandler = std::shared_ptr<MapHandler>(new MapHandler(m_currentMap, m_jsonHandler));
	Camera::Mapsize = m_mapHandler->GetMapSize();
	m_collisionManager.SetMapTiles(m_mapHandler->Tiles());

	m_celist = EntityFactory::GetEntities(m_currentMap, m_jsonHandler);
	m_eventManager.SetEntitiesMap(&m_celist);
	m_collisionManager.SetEntities(&m_celist);

	// THIS IS NOT GOOD - THE PLAYER NEEDS TO BE FOUND, NOT JUST BE ID 1
	// NVM THIS IS GOOD
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

SceneWorld::~SceneWorld()
{
	FontManager::GetInstance().RemoveFont(m_fontFPS);
	//FontManager::GetInstance().RemoveFont(m_fontTitle);
}


//Loads GL resources
void SceneWorld::LoadAllResources()
{
	//for (Drawable* x : *m_objList)
	//	x->LoadGLResources();

	//m_test->LoadGLResources();
	//m_mapHandler->Graphics()->LoadGLResources();

	m_resources_loaded = true;
}

void SceneWorld::UnloadAllResources()
{
	//for (Drawable* x : *m_objList)
	//	x->UnloadExternalResources();

	m_resources_loaded = false;
}

void SceneWorld::ManageInput()
{
	InputManager::GetInstance().SetupFrameKeys();

	if (InputManager::GetInstance().FrameKeyStatus('B', AnyRelease))
		m_bloomEffect = !m_bloomEffect;

	if (InputManager::GetInstance().FrameKeyStatus('F', AnyRelease))
		m_fade.SetFade(true);
	if (InputManager::GetInstance().FrameKeyStatus('G', AnyRelease))
		m_fade.SetFade(false);

	if (InputManager::GetInstance().FrameKeyStatus('T', AnyRelease))
		m_World->AddTranslation(0, 0, 1);

	if (InputManager::GetInstance().FrameKeyStatus('Y', AnyRelease))
		m_World->AddTranslation(0, 0, -1);

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

	static float bloomint = 0.3f;
	if (InputManager::GetInstance().FrameKeyStatus('U')) {
		bloomint += 0.2f;
		CombineEffect::GetInstance().SetIntensity(bloomint);
	}
	if (InputManager::GetInstance().FrameKeyStatus('J')) {
		bloomint -= 0.2f;
		CombineEffect::GetInstance().SetIntensity(bloomint);
	}
}

SceneGenData SceneWorld::Act()
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

	if (!m_fade.IsDone() || result.id == 0 && result.sceneType != ST_Battle)
	{
		result.id = 0;
		return result;
	}
	Brb();
	return result;
}

void SceneWorld::Draw()
{
	RenderPass();
}

void SceneWorld::Interact()
{
	if (InputManager::GetInstance().FrameKeyStatus(' ', KeyStatus::KeyPressed) && m_player) {
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
		// If he wasn't just touched...
		if (!entity->_justTouched)
			// ...push back the event
			for (auto x : *entity->GetQueues())
				if (x->GetActivationType() == AT_Touch)
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

void SceneWorld::RenderPass()
{
	// Allow modifications to stencil buffer
	glStencilMask(0xFF);
	// Clear stencil buffer to all 0's
	glClearStencil(0x00);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Don't allow modifications to stencil buffer
	glStencilMask(0x00);

	// Setup world trans and change for effects
	SetOrthoStuffs();

	EffectManager::GetInstance().Enable(E_Basic);

	// Set the renders
	m_mapHandler->SetRender();
	for (auto it : m_celist)
		it.second->SetRender();
	FontManager::GetInstance().SetRender();

	// Enable fade
	if (!m_fade.IsDone())
		m_fade.Begin();

	// Enable bloom
	if (m_bloomEffect)
		Bloom::GetInstance().Begin();

	// Draw
	Renderer::GetInstance().Draw();

	// End bloom
	if (m_bloomEffect)
		Bloom::GetInstance().End(false);

	// Set translation back to real value, not adjusted by tilesize (64)
	m_World->SetTranslation(m_backupTrans);
}

void SceneWorld::SetAudioPosition()
{
	if (m_player != NULL)
		SoundManager::GetInstance().SetListenerPosition(m_player->Physics()->Position(), m_player->Physics()->Velocity());
	else
		SoundManager::GetInstance().SetListenerPosition();
}

void SceneWorld::SetNextScene(SceneGenData sgd)
{
	NextScene = sgd;
}

void SceneWorld::SetOrthoStuffs()
{
	// Adjust the shaders and camera to new screen size
	if (OrthoProjInfo::GetRegularInstance().changed)
	{
		OrthoProjInfo* o = &OrthoProjInfo::GetRegularInstance();
		m_World->SetOrthoProj(o);
		m_World->SetTranslation(-o->Right / o->Size, -o->Top  / o->Size, 0);

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

	EffectManager::GetInstance().SetWorldTrans(&m_World->GetWOTrans().m[0][0], &m_World->GetWOTransNoTranslate().m[0][0]);
	EffectManager::GetInstance().ResetWorldUpdateFlag();
}

