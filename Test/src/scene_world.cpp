#include "scene_world.h"

SceneWorld::SceneWorld() : m_acceptInput(false)
{
	Init();
}

bool SceneWorld::Init()
{


	//Setup viewport to fit the window size
	glViewport(0, 0, (GLsizei)(glutGet(GLUT_WINDOW_WIDTH)), (GLsizei)(glutGet(GLUT_WINDOW_HEIGHT)));

	m_bloomEffect = false;

	m_camera = new Camera((float)glutGet(GLUT_WINDOW_WIDTH), (float)glutGet(GLUT_WINDOW_HEIGHT));
	m_camera->Pos.z = -25;

	m_World = new Transformation();

	BasicEffect::GetInstance();
	//PointLightShadowEffect::GetInstance().SetColorTextureUnit(0);
	//PointLightShadowEffect::GetInstance().SetShadowMapTextureUnit(1);


	m_clist = std::vector<Drawable*>();
	m_player = new Actor(Vector3f(0, 0, 4.0f), "TILE", "res/player.png");
	//m_map = new Map();

	//Lists
	m_objList = new std::vector<Drawable*>();
	std::vector<ResourceUser*> temp = std::vector<ResourceUser*>();

	Actor* test = new Actor(Vector3f(1, 1, 4.0f), "TILE", "res/player.png");
	m_objList->push_back(test);
	temp.push_back(test);

	Actor* test2 = new Actor(Vector3f(10, 5, 4.0f), "TILE", "res/player.png");
	m_objList->push_back(test2);
	temp.push_back(test2);

	Actor* test3 = new Actor(Vector3f(4, 12, 4.0f), "TILE", "res/player.png");
	m_objList->push_back(test3);
	temp.push_back(test3);

	m_objList->push_back(m_player);
	temp.push_back(m_player);
	//m_objList->push_back(m_map);
	//temp.push_back(m_map);

	//temp.push_back(new Actor(Vector3f(1, 1, 4.0f), "TILE", "res/player.png"));

	//populate clist
	for (auto x : *m_objList)
	{
		if (x->IsContainer() && x->GetContainedObjects() != NULL)
		{
			for (auto y : *x->GetContainedObjects())
			{
				m_clist.push_back(y);
			}
		}
		else
		{
			m_clist.push_back(x);
		}
	}

	ResourceManager::GetInstance().LoadAllExternalResources(&temp);
	LoadAllResources();

	m_pause = false;
	m_acceptInput = true;


	m_mapHandler = new MapHandler();

	m_test2 = new Entity(true);
	m_test2->Physics()->SetPosition(Vector3f(0, 0, 4.5f));
	m_test2->Physics()->AbsolutePosition(Vector3f(0, 0, 4.5f));
	m_test3 = new Entity();
	m_test3->Physics()->SetPosition(Vector3f(2.5f, 2, 4.5f));
	m_test3->Physics()->AbsolutePosition(Vector3f(2.5f, 2, 4.5f));

	m_celist = std::vector<Entity*>();
	m_celist.push_back(m_test2);
	m_celist.push_back(m_test3);

	return true;
}

//Loads GL resources
void SceneWorld::LoadAllResources()
{
	for (Drawable* x : *m_objList)
		x->LoadGLResources();

	//m_test->LoadGLResources();
	//m_mapHandler->Graphics()->LoadGLResources();

	m_resources_loaded = true;
}

void SceneWorld::UnloadAllResources()
{
	for (Drawable* x : *m_objList)
		x->UnloadExternalResources();

	m_resources_loaded = false;
}

void SceneWorld::ManageInput()
{
	InputManager::GetInstance().SetupFrameKeys();

	//std::list<std::pair<unsigned int, KeyStatus>>* keys = InputManager::GetInstance().GetKeys();
	//std::map<unsigned int, bool>* heldKeys = InputManager::GetInstance().GetHeldKeys();

	//std::map<std::pair<unsigned int, KeyStatus>, bool> keyMap = std::map<std::pair<unsigned int, KeyStatus>, bool>();
	//for (auto a : *keys)
	//	keyMap.emplace(a, false);

	//if (!m_acceptInput)
	//	return;

	//if (keyMap.count(std::pair<unsigned int, KeyStatus>('p', KeyPressed)))
	//	m_pause = !m_pause;

	//if (keyMap.count(std::pair<unsigned int, KeyStatus>('o', KeyPressed)))
	//	m_numFrames++;

	////Manage MOVE and JUMP here
	//if (m_numFrames > 0 || !m_pause)
	//	m_player->Move(
	//		heldKeys->count(GLUT_KEY_UP + InputManager::SpecialKeyValue),
	//		heldKeys->count(GLUT_KEY_DOWN + InputManager::SpecialKeyValue),
	//		heldKeys->count(GLUT_KEY_LEFT + InputManager::SpecialKeyValue),
	//		heldKeys->count(GLUT_KEY_RIGHT + InputManager::SpecialKeyValue));

	//if (keyMap.count(std::pair<unsigned int, KeyStatus>(' ', KeyStatus::KeyPressed)))
	//	m_player->Jump(false);
	//else if (keyMap.count(std::pair<unsigned int, KeyStatus>(' ', Release)))
	//	m_player->Jump(true);

	//if (keyMap.count(std::pair<unsigned int, KeyStatus>('b', KeyPressed)))
	//	m_bloomEffect = !m_bloomEffect;

	//static float bloomint = 0.3f;
	//if (keyMap.count(std::pair<unsigned int, KeyStatus>('u', KeyPressed))) {
	//	bloomint += 0.2f;
	//	CombineEffect::GetInstance().SetIntensity(bloomint);
	//}
	//if (keyMap.count(std::pair<unsigned int, KeyStatus>('j', KeyPressed))) {
	//	bloomint -= 0.2f;
	//	CombineEffect::GetInstance().SetIntensity(bloomint);
	//}

	//if (heldKeys->count(GLUT_KEY_F5 + InputManager::SpecialKeyValue))
	//	m_camAngle += 0.1f;
	//else if (heldKeys->count(GLUT_KEY_F6 + InputManager::SpecialKeyValue))
	//	m_camAngle -= 0.1f;

	////Free memory here
	//delete keys;
}

Scene* SceneWorld::Act()
{
	if (!m_resources_loaded)
		LoadAllResources();

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

	//SIMPLE RENDER SETUP
	//if (!m_pause)
	//	Update();

	//DRAW
	Draw();

	return &SceneWorld::GetInstance();
}

void SceneWorld::Draw()
{
	//ShadowMapPass();
	RenderPass();
}

void SceneWorld::Update()
{
	Animation::AnimationCounter((float)ElapsedTime::GetInstance().GetElapsedTime());
	//DesiredMove
	//for (auto it : *m_objList)
	//	it->DesiredMove();

	for (auto it : m_celist)
		it->Physics()->DesiredMove();

	//Collision
	//Physics_2D::Collision(&m_clist);
	Physics_2D::Collision(&m_celist, m_mapHandler);

	//Update
	//for (auto it : *m_objList)
	//	it->Update();

	for (auto it : m_celist)
		it->Update();

	m_mapHandler->Update();

	//m_test2->Update();

	//std::cout << m_player->GetDirection() << std::endl;
	std::cout << /*m_player->Position().x << ", " << m_player->Position().y << ", " <<*/ m_test2->Physics()->Position().z << std::endl;// << ", " << m_clist.at(1)->GetMoveBoundingBox().Get(AABB::Down) << ", " << m_clist.at(1)->GetMoveBoundingBox().Get(AABB::Close) << std::endl;

	m_World->Follow(m_test2->Physics()->Position(), Vector3f(32, 18, 0));
	//m_camera->Update(m_player->Position());//this needs to change LOLOLOLOL
}

void SceneWorld::RenderPass()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static bool drawinited = false;
	if (!drawinited)
	{
		//m_camera->Up = Vector3f(0, 1, 0);
		//m_World->SetCamera(*m_camera);
		m_World->SetOrthoProj(&OrthoProjInfo::GetRegularInstance());
		m_World->SetTranslation(OrthoProjInfo::GetRegularInstance().Left, OrthoProjInfo::GetRegularInstance().Bottom, 0);
		//m_World->SetRotation(m_camAngle, 0.0f, 0.0f);

		BasicEffect::GetInstance().Enable();
		BasicEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		BlurEffect::GetInstance().Enable();
		BlurEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		BloomEffect::GetInstance().Enable();
		BloomEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		CombineEffect::GetInstance().Enable();
		CombineEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		TransparencyEffect::GetInstance().Enable();
		TransparencyEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);
		drawinited = true;
	}

	//m_World->SetTranslation(-m_player->Position().x, -m_player->Position().y, 0);

	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);

	if (!m_bloomEffect)
	{
		//for (auto it : *m_objList)
		//	it->Draw();

		//m_test->Update();
		//m_test->Draw();

		m_mapHandler->Draw();

		for (auto it : m_celist)
			it->Draw();
		//m_test2->Draw();
	}
	else
		//Draw blur
	{
		//m_trail.Begin();
		m_bloom.Begin();

		for (auto it : *m_objList)
			it->Draw();

		m_bloom.End();
		//m_trail.End();
	}




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
