#include "scene_world.h"

SceneGenData SceneWorld::NextScene = SceneGenData();

SceneWorld::SceneWorld(unsigned int map_id) : m_acceptInput(false), m_currentMap(map_id), m_drawinited(false)
{
	Init();
	SoundManager::GetInstance();
	m_fade.SetFade(true);
}

bool SceneWorld::Init()
{
	//SoundManager::GetInstance().CreateBuffer("res/audio/fx/washeslow.wav");
	//unsigned int m_source = SoundManager::GetInstance().CreateSource();
	//SoundManager::GetInstance().CreateBuffer("res/audio/fx/swish_2.wav");
	//SoundManager::GetInstance().Play(m_source, "res/audio/fx/swish_2.wav");

	NextScene = SceneGenData();
	m_jsonHandler = std::shared_ptr<JsonHandler>(new JsonHandler(m_currentMap));

	m_bloomEffect = false;

	OrthoProjInfo::GetRegularInstance().changed = true;
	m_World = std::shared_ptr<Transformation>(new Transformation());

	BasicEffect::GetInstance();

	m_pause = false;
	m_acceptInput = true;

	m_mapHandler = std::shared_ptr<MapHandler>(new MapHandler(m_currentMap, m_jsonHandler));
	m_collisionManager.SetMapTiles(m_mapHandler->Tiles());

	m_celist = EntityFactory::GetEntities(m_currentMap, m_jsonHandler);
	m_eventManager.SetEntitiesMap(&m_celist);
	m_collisionManager.SetEntities(&m_celist);

	// THIS IS NOT GOOD - THE PLAYER NEEDS TO BE FOUND, NOT JUST BE ID 1
	// NVM THIS IS GOOD
	if (m_celist.count(1))
		m_player = m_celist.at(1);

	m_fontTitle = FontManager::GetInstance().AddFont(false, false);
	m_fontFPS = FontManager::GetInstance().AddFont(true, false, true);
	FontManager::GetInstance().SetScale(m_fontFPS, 0.5f, 0.5f);

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
	FontManager::GetInstance().RemoveFont(m_fontTitle);
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

	if (InputManager::GetInstance().FrameKeyStatus('b', AnyRelease))
		m_bloomEffect = !m_bloomEffect;

	if (InputManager::GetInstance().FrameKeyStatus('f', AnyRelease))
		m_fade.SetFade(true);
	if (InputManager::GetInstance().FrameKeyStatus('g', AnyRelease))
		m_fade.SetFade(false);

	if (InputManager::GetInstance().FrameKeyStatus('t', AnyRelease))
		m_World->AddTranslation(0, 0, 1);

	if (InputManager::GetInstance().FrameKeyStatus('y', AnyRelease))
		m_World->AddTranslation(0, 0, -1);

	if (InputManager::GetInstance().FrameKeyStatus('p', AnyRelease))
		m_pause = !m_pause;

	static float bloomint = 0.3f;
	if (InputManager::GetInstance().FrameKeyStatus('u')) {
		bloomint += 0.2f;
		CombineEffect::GetInstance().SetIntensity(bloomint);
	}
	if (InputManager::GetInstance().FrameKeyStatus('j')) {
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

	if (!m_fade.IsDone())
	{
		result.id = 0;
		return result;
	}
	return result;
}

void SceneWorld::Draw()
{
	//ShadowMapPass();
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
	std::vector<std::shared_ptr<Entity>> collided = m_collisionManager.CalculateCollision();//Physics_2D::Collision(&m_celist, m_mapHandler);
	for (auto entity : collided)
	{
		for (auto x : *entity->GetQueues())
			if (x->GetActivationType() == AT_Touch)
				m_eventManager.PushBack(x);
	}

	//Update
	for (auto it : m_celist)
		it.second->Update();

	m_mapHandler->Update();

	SetAudioPosition();
	SoundManager::GetInstance().Update();

	if (m_player)
		m_World->Follow(m_player->Physics()->Position(), m_mapHandler->GetMapSize());

	//Display FPS
	FontManager::GetInstance().SetText(m_fontFPS, std::to_string(ElapsedTime::GetInstance().GetFPS()), Vector3f(0, OrthoProjInfo::GetRegularInstance().Top / 32.0f - 0.5f, 0));

	srand(clock());
	FontManager::GetInstance().Update(ElapsedTime::GetInstance().GetElapsedTime());

	return NextScene;
}

void SceneWorld::RenderPass()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetOrthoStuffs();

	BasicEffect::GetInstance().Enable();

	// Set the renders
	m_mapHandler->SetRender();
	//m_particles.SetRender();

	for (auto it : m_celist)
		it.second->SetRender();
	FontManager::GetInstance().SetRender();

	if (!m_fade.IsDone())
		m_fade.Begin();

	Effect::SetWorldPosition(*m_World->GetWOTrans().m);

	// BLOOM
	if (m_bloomEffect)
		m_bloom.Begin();

	// DRAW
	Renderer::GetInstance().Draw();

	// BLOOM END
	if (m_bloomEffect)
		m_bloom.End(false);

	//Debug tile outline drawing
	if (Globals::DEBUG_DRAW_TILE_OUTLINES)
	{
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		for (int x = 1; x < 32; x++)
		{
			glVertex3f((GLfloat)x, (GLfloat)0, (GLfloat)-8);
			glVertex3f((GLfloat)x, (GLfloat)18, (GLfloat)-8);
		}
		for (int y = 1; y < 18; y++)
		{
			glVertex3f((GLfloat)0, (GLfloat)y, (GLfloat)-8);
			glVertex3f((GLfloat)32, (GLfloat)y, (GLfloat)-8);
		}
		glEnd();
	}

	m_World->SetTranslation(m_backupTrans);

	GLUTBackendSwapBuffers();
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
	if (OrthoProjInfo::GetRegularInstance().changed)
	{
		m_World->SetOrthoProj(&OrthoProjInfo::GetRegularInstance());
		m_World->SetTranslation(OrthoProjInfo::GetRegularInstance().Left, OrthoProjInfo::GetRegularInstance().Bottom, 0);

		BasicEffect::GetInstance().Enable();
		BasicEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		Effect::SetTileSize(OrthoProjInfo::GetRegularInstance().Size);
		FadeEffect::GetInstance().Enable();
		FadeEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		Effect::SetTileSize(OrthoProjInfo::GetRegularInstance().Size);

		for (int i = 0; i < 30; i++)
			m_World->Follow(m_player->Physics()->Position(), m_mapHandler->GetMapSize());

		OrthoProjInfo::GetRegularInstance().changed = false;
	}

	m_backupTrans = m_World->GetTranslation();

	Vector3f temp = m_backupTrans;
	temp.x *= OrthoProjInfo::GetRegularInstance().Size;
	temp.y *= OrthoProjInfo::GetRegularInstance().Size;
	// Floor so we dont move the camera half a pixel and fuck up the graphics
	temp.x = floorf(temp.x);
	temp.y = floorf(temp.y);
	m_World->SetTranslation(temp);
}

