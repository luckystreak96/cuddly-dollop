#include "scene_world.h"

SceneWorld::SceneWorld() : m_acceptInput(false)
{
	Init();
	SoundManager::GetInstance();
}

bool SceneWorld::Init()
{
	JsonHandler::LoadJsonFromFile(DATA_FILE);

	//Setup viewport to fit the window size
	glViewport(0, 0, (GLsizei)(glutGet(GLUT_WINDOW_WIDTH)), (GLsizei)(glutGet(GLUT_WINDOW_HEIGHT)));

	m_bloomEffect = true;

	m_camera = new Camera((float)glutGet(GLUT_WINDOW_WIDTH), (float)glutGet(GLUT_WINDOW_HEIGHT));
	m_camera->Pos.z = -25;

	m_World = new Transformation();

	BasicEffect::GetInstance();

	m_pause = false;
	m_acceptInput = true;

	m_currentMap = 0;
	m_mapHandler = new MapHandler(0);

	m_celist = EntityFactory::GetEntities(m_currentMap);
	m_eventManager.SetEntitiesMap(&m_celist);

	// THIS IS NOT GOOD - THE PLAYER NEEDS TO BE FOUND, NOT JUST BE ID 1
	if (m_celist.count(1))
		m_player = m_celist.at(1);

	m_fontTitle = FontManager::GetInstance().AddFont(false, false);
	m_fontFPS = FontManager::GetInstance().AddFont(true, false, true);
	FontManager::GetInstance().SetScale(m_fontFPS, 0.5f, 0.5f);
	//FontManager::GetInstance().SetText(m_fontTitle, "IT WORKS!");
	for (auto e : m_celist)
	{
		for (auto x : EventFactory::LoadEvent(m_currentMap, e.first))
			if (x->GetActivationType() == AT_Autorun)
				m_eventManager.PushBack(x);
	}

	return true;
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

Scene* SceneWorld::Act()
{
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
		Update();
	}

	//DRAW
	Draw();

	return &SceneWorld::GetInstance();
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

		Entity* inter = NULL;
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
			//m_player->Communicate(inter->Input()->Interact(&m_eventQueue, inter->GetID()));
			inter->Graphics()->SetDirection(m_player->Graphics());
			TriggerEvents(inter->GetID());
		}
	}
}

void SceneWorld::TriggerEvents(unsigned int entity_id)
{
	for (auto x : EventFactory::LoadEvent(m_currentMap, entity_id))
		if (x->GetActivationType() == AT_Interact)
			m_eventManager.PushBack(x);
}

void SceneWorld::Update()
{
	// Needs to be called here so the EventQueues can set render
	Renderer::GetInstance().Empty();
	Animation::AnimationCounter((float)ElapsedTime::GetInstance().GetElapsedTime());
	Interact();
	m_eventManager.Update(ElapsedTime::GetInstance().GetElapsedTime());

	for (auto it : m_celist)
		it.second->Physics()->DesiredMove();

	//Collision
	std::vector<Entity*> collided = Physics_2D::Collision(&m_celist, m_mapHandler);
	for (auto x : collided)
	{
		for (auto x : EventFactory::LoadEvent(m_currentMap, x->GetID()))
			if (x->GetActivationType() == AT_Touch)
				m_eventManager.PushBack(x);
	}

	//Update
	for (auto it : m_celist)
		it.second->Update();

	m_mapHandler->Update();

	SetAudioPosition();
	//SoundManager::GetInstance().SetListenerOrientation(((PlayerGraphicsComponent*)m_player->Graphics())->GetDirection());

	//std::cout << m_player->GetDirection() << std::endl;
	//std::cout << /*m_player->Position().x << ", " << m_player->Position().y << ", " <<*/ m_player->Physics()->Position().z << std::endl;// << ", " << m_clist.at(1)->GetMoveBoundingBox().Get(AABB::Down) << ", " << m_clist.at(1)->GetMoveBoundingBox().Get(AABB::Close) << std::endl;

	if (m_player)
		m_World->Follow(m_player->Physics()->Position(), m_mapHandler->GetMapSize()/*Vector3f(32, 18, 0)*/);
	//m_camera->Update(m_player->Position());//this needs to change LOLOLOLOL

	//Display FPS
	FontManager::GetInstance().SetText(m_fontFPS, std::to_string(ElapsedTime::GetInstance().GetFPS()), Vector3f(0, 15.5f, 0));
	//FontManager::GetInstance().GetFont(m_fontFPS)->GetGraphics()->SetScale(Vector3f(0.5, 0.5, 1));

	srand(clock());
	//FontManager::GetInstance().ChangeLetter(m_fontTitle, 0, rand() % 4 == 1 ? '_' : 'I');
	FontManager::GetInstance().Update(ElapsedTime::GetInstance().GetElapsedTime());
}

void SceneWorld::RenderPass()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static bool drawinited = false;
	if (!drawinited)
	{
		m_World->SetOrthoProj(&OrthoProjInfo::GetRegularInstance());
		m_World->SetTranslation(OrthoProjInfo::GetRegularInstance().Left, OrthoProjInfo::GetRegularInstance().Bottom, 0);
		//m_World->SetRotation(0.0f, 0.0f, 0.1f);

		BasicEffect::GetInstance().Enable();
		BasicEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		BlurEffect::GetInstance().Enable();
		BlurEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		BloomEffect::GetInstance().Enable();
		BloomEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		BloomEffect::GetInstance().Enable(BloomEffect::GetInstance().GetDark());
		BloomEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		CombineEffect::GetInstance().Enable();
		CombineEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		TransparencyEffect::GetInstance().Enable();
		TransparencyEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		HeightEffect::GetInstance().Enable();
		HeightEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);

		drawinited = true;
	}

	BasicEffect::GetInstance().Enable();
	Effect::SetWorldPosition(*m_World->GetWOTrans().m);

	if (!m_bloomEffect)
	{
		m_mapHandler->SetRender();

		for (auto it : m_celist)
			it.second->SetRender();
		Renderer::GetInstance().Draw();
	}
	else
	{
		m_bloom.Begin();

		m_mapHandler->SetRender();

		for (auto it : m_celist)
			it.second->SetRender();

		FontManager::GetInstance().SetRender();

		Renderer::GetInstance().Draw();

		bool darkBloom = true;
		m_bloom.End(darkBloom);

	}

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

	GLUTBackendSwapBuffers();
}

void SceneWorld::SetAudioPosition()
{
	if (m_player != NULL)
		SoundManager::GetInstance().SetListenerPosition(m_player->Physics()->Position(), m_player->Physics()->Velocity());
	else
		SoundManager::GetInstance().SetListenerPosition();
}