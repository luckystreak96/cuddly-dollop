#include "scene_world.h"

SceneWorld::SceneWorld() : m_acceptInput(false)
{
	Init();
}

bool SceneWorld::Init()
{
	//Setup viewport to fit the window size
	float ratio = WINDOW_WIDTH / WINDOW_HEIGHT;
	glViewport(0, 0, WINDOW_WIDTH * ratio, WINDOW_HEIGHT * ratio);

	m_shadowMapSize = glutGet(GLUT_WINDOW_WIDTH) > glutGet(GLUT_WINDOW_HEIGHT) ? glutGet(GLUT_WINDOW_WIDTH) : glutGet(GLUT_WINDOW_HEIGHT);

	m_camera = new Camera(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	m_camera->Pos.z = -25;

	m_World = new Transformation();

	BasicEffect::GetInstance();
	//PointLightShadowEffect::GetInstance().SetColorTextureUnit(0);
	//PointLightShadowEffect::GetInstance().SetShadowMapTextureUnit(1);

	Model::GetInstance().init("res/models.data");


	m_player = new Actor(Vector3f(0, 0, 4.0f), "TILE", "res/mushroom.png");
	m_map = new Map();

	//Lists
	m_objList = new std::vector<Drawable*>();
	std::vector<ResourceUser*> temp = std::vector<ResourceUser*>();
	m_objList->push_back(m_player);
	temp.push_back(m_player);
	m_objList->push_back(m_map);
	temp.push_back(m_map);
	for (auto t : m_map->Tiles())
	{
		m_objList->push_back(t);
		temp.push_back(t);
	}
		
	ResourceManager::GetInstance().LoadAllExternalResources(&temp);
	LoadAllResources();

	m_pause = false;
	m_acceptInput = true;

	return true;
}

//Loads GL resources
void SceneWorld::LoadAllResources()
{
	for (Drawable* x : *m_objList)
		x->LoadGLResources();

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
	std::list<std::pair<unsigned int, KeyStatus>>* keys = InputManager::GetInstance().GetKeys();
	std::map<unsigned int, bool>* heldKeys = InputManager::GetInstance().GetHeldKeys();

	std::map<std::pair<unsigned int, KeyStatus>, bool> keyMap = std::map<std::pair<unsigned int, KeyStatus>, bool>();
	for (auto a : *keys)
		keyMap.emplace(a, false);

	if (!m_acceptInput)
		return;

	if (keyMap.count(std::pair<unsigned int, KeyStatus>('p', KeyPressed)))
		m_pause = !m_pause;

	//Manage MOVE and JUMP here
	m_player->Move(
		heldKeys->count(GLUT_KEY_UP + InputManager::SpecialKeyValue),
		heldKeys->count(GLUT_KEY_DOWN + InputManager::SpecialKeyValue),
		heldKeys->count(GLUT_KEY_LEFT + InputManager::SpecialKeyValue),
		heldKeys->count(GLUT_KEY_RIGHT + InputManager::SpecialKeyValue));

	if (keyMap.count(std::pair<unsigned int, KeyStatus>(' ', KeyPressed)))
		m_player->Jump(false);
	else if (keyMap.count(std::pair<unsigned int, KeyStatus>(' ', Release)))
		m_player->Jump(true);

	if (heldKeys->count(GLUT_KEY_F5 + InputManager::SpecialKeyValue))
		m_camAngle += 0.1f;
	else if (heldKeys->count(GLUT_KEY_F6 + InputManager::SpecialKeyValue))
		m_camAngle -= 0.1f;

	//Free memory here
	delete keys;
}

Scene* SceneWorld::Act()
{
	if (!m_resources_loaded)
		LoadAllResources();

	ManageInput();

	//RENDER SETUP
	if (!m_pause)
		Update();
	
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
	//DesiredMove
	for(auto it : *m_objList)
		it->DesiredMove();

	//Collision
	//Physics_Vertex::VertexCollision(m_objList);

	//Update
	for(auto it : *m_objList)
		it->Update();

	//std::cout << m_player->Position().x << ", " << m_player->Position().y << ", " << m_player->Position().z << std::endl;

	m_camera->Update(m_player->Position());//this needs to change LOLOLOLOL
}

void SceneWorld::ShadowMapPass()
{
	static CameraDirection gCameraDirections[NUM_OF_LAYERS] =
	{
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, Vector3f(1.0f, 0.0f, 0.0f),  Vector3f(0.0f, -1.0f, 0.0f) },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Vector3f(-1.0f, 0.0f, 0.0f), Vector3f(0.0f, -1.0f, 0.0f) },
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Vector3f(0.0f, 1.0f, 0.0f),  Vector3f(0.0f, 0.0f, -1.0f) },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Vector3f(0.0f, -1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f) },
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Vector3f(0.0f, 0.0f, -1.0f),  Vector3f(0.0f, -1.0f, 0.0f) },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0.0f, -1.0f, 0.0f) }
	};

	glViewport(0, 0, m_shadowMapSize, m_shadowMapSize);

	glCullFace(GL_BACK);

	ShadowMapEffect::GetInstance().Enable();

	Transformation p = Transformation();
	p.SetPersProjInfo(&PersProjInfo::GetShadowInstance());

	//To be changed to fit lots o lights
	Vector3f pos = PointLightShadowEffect::GetInstance().GetPointLightLocation(0);
	ShadowMapEffect::GetInstance().SetLightWorldPos(pos);

	glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

	for (unsigned int i = 0; i < NUM_OF_LAYERS; i++) {
		ShadowMapFBO::GetInstance().BindForWriting(gCameraDirections[i].CubemapFace);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		p.SetCamera(gCameraDirections[i].Up, pos, gCameraDirections[i].Target);

		for (auto it : *m_objList)
			it->DrawShadowMap(p);
	}
}

void SceneWorld::RenderPass()
{
	//glCullFace(GL_FRONT);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//PointLightShadowEffect::GetInstance().Enable();
	//ShadowMapFBO::GetInstance().BindForReading(SHADOW_TEXTURE_UNIT);

	//glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	//m_camera->Target = Vector3f(0, 0, 1);
	//m_camera->Up = Vector3f(0, 1, 0);
	//m_World->SetCamera(*m_camera);
	m_World->SetOrthoProj(&OrthoProjInfo::GetRegularInstance());
	m_World->SetTranslation(OrthoProjInfo::GetRegularInstance().Left, OrthoProjInfo::GetRegularInstance().Bottom, 0);
	//m_World->SetRotation(m_camAngle, 0.0f, 0.0f);

	//PointLightShadowEffect::GetInstance().Move(&m_World->GetWorldTrans());
	//PointLightShadowEffect::GetInstance().SetWorldPosition(*m_World->GetTrans().m);
	//PointLightShadowEffect::GetInstance().SetEyeWorldPos(m_camera->Pos);
	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetWorldPosition(*m_World->GetWOTrans().m);


	for (auto it : *m_objList)
		it->Draw();

	GLUTBackendSwapBuffers();
}
