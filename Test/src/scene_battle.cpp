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
#include "fighterFactory.h"
#include "particleManager.h"
#include "battleAnimationManager.h"

SceneBattle::SceneBattle() : m_zoom(false)
{
	m_currentMap = 3;
	m_fighters.push_back(Fighter_ptr(FighterFactory::BuildBaseAlly()));
	m_fighters.push_back(Fighter_ptr(FighterFactory::BuildBaseAlly()));
	m_fighters.push_back(Fighter_ptr(FighterFactory::BuildBaseAlly()));
	//_fighters.push_back(Fighter_ptr(FighterFactory::BuildBaseAlly()));
	for (auto x : BattleData::Party)
		m_fighters.push_back(x);

	m_fighters.push_back(Fighter_ptr(FighterFactory::BuildFighter(1)));
	m_fighters.push_back(Fighter_ptr(FighterFactory::BuildFighter(1)));
	m_fighters.push_back(Fighter_ptr(FighterFactory::BuildFighter(1)));
	m_fighters.push_back(Fighter_ptr(FighterFactory::BuildFighter(1)));
	//_fighters.push_back(Fighter_ptr(FighterFactory::BuildBaseEnemy()));
	//_fighters.push_back(Fighter_ptr(FighterFactory::BuildBaseEnemy()));
	//_fighters.push_back(Fighter_ptr(FighterFactory::BuildBaseEnemy()));
	Init();
	m_battle = BattleManager(m_fighters);

	std::map<int, Actor_ptr> actors;

	for (auto& x : m_actors)
	{
		actors.emplace(x->GetId(), x);
	}

	m_battle.GetGraphics()->SetActors(actors);
	m_battle.SetupHUD();

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
	InputManager::GetInstance().PopLockLevel();

	OrthoProjInfo::GetRegularInstance().changed = true;

	m_mapHandler = std::shared_ptr<MapHandler>(new MapHandler(m_currentMap, m_jsonHandler));
	m_camera._mapsize = m_mapHandler->GetMapSize();
	m_camera.ForcePosition(m_camera.MapCenter() - Vector3f(2.0f, 2.0f, 0));
	m_camera.SetFollow(m_camera.MapCenter());
	m_camera.ForceScale(Vector3f(1.75f));
	m_camera.SetScale(Vector3f(1.0f));
	//m_camera._lerperTrans.MaxVelocity = 0.1f;
	m_camera._style = CAMSTYLE_FollowDad;

	std::vector<Actor_ptr> party;
	std::vector<Actor_ptr> enemies;
	int a = 0;
	int b = 4;
	float size = OrthoProjInfo::GetRegularInstance().Size;

	// Set battlefield position for proper iteration
	for (auto& x : m_fighters)
	{
		Actor_ptr actor = Actor_ptr(new Actor());
		actor->SetTexture(x->GetSprite());

		if (x->Team == 0)
		{
			x->_BattleFieldPosition = a++;
			actor->SetId(x->_BattleFieldPosition);
			party.push_back(actor);
		}
		else
		{
			x->_BattleFieldPosition = b++;
			actor->SetId(x->_BattleFieldPosition);
			enemies.push_back(actor);
		}

		m_actors.push_back(actor);
	}




	float startY = (4.25f + (1.25f * (4 - party.size())) / 2.0f);
	//float startY = 4.25f;

	for (int i = 0; i < party.size(); i++)
	{
		Vector3f position = Vector3f((4.0f + i * 0.25f) * size, (startY + i * 1.25f) * size, 4.0f);
		party.at(i)->SetPhysics(position, Vector3f());
		//if (!Transformation::perspectiveOrtho)
			party.at(i)->AdjustHeightForAngle();
		party.at(i)->WaitPosition = party.at(i)->GetPos();
		party.at(i)->ActivePosition = party.at(i)->GetPos() + Vector3f(1, 0, 0) * size;
		party.at(i)->_row = AE_Right; // make player face right cuz girl looks bad looking down
		party.at(i)->_default_animation = AE_Right; // make player face right cuz girl looks bad looking down
	}

	startY = (4.25f + (1.25f * (4 - enemies.size())) / 2.0f);

	for (int i = 0; i < enemies.size(); i++)
	{
		Vector3f position = Vector3f((13.5f - i * 0.25f) * size, (startY + i * 1.25f) * size, 4.0f);
		enemies.at(i)->SetPhysics(position, Vector3f());
		//if (!Transformation::perspectiveOrtho)
			enemies.at(i)->AdjustHeightForAngle();
		enemies.at(i)->WaitPosition = enemies.at(i)->GetPos();
		enemies.at(i)->ActivePosition = enemies.at(i)->GetPos() + Vector3f(-1, 0, 0) * size;
		// Set to right cause the sprites are flipped
		enemies.at(i)->_row = AE_Right;
		enemies.at(i)->_default_animation = AE_Right;
		enemies.at(i)->GetModelMat()->SetScale(-1, 1, 1);
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
	//m_battle._hud.Destroy();

	//for (auto& x : m_fighters)
	//	for (auto& y : x->_Fighter->m_skills)
	//	{
	//		y->_fighters->clear();
	//		y->_targets.clear();
	//		y->_anims = nullptr;
	//	}
}

void SceneBattle::ManageInput()
{
	Scene::ManageInput();

	if (InputManager::GetInstance().FrameKeyStatus(A_Accept, KeyStatus::KeyPressed))
	{
		if (!m_battle.Animating() && m_battle._postBattleState != PBS_FightingInProgress && m_battle._postBattleState != PBS_PostBattleComplete)
			m_battle._postBattleState = (PostBattleState)((int)m_battle._postBattleState + 1);

		if (m_fade.IsDone() && m_battle._postBattleState == PBS_PostBattleComplete)
		{
			if (m_battle.FindWinner() == 0)
			{
				m_fade.SetFade(false);
				NextScene.scene = _prevScene;
				NextScene.sceneType = ST_World;
				NextScene.id = 1;
			}
			else if (m_battle.FindWinner() != 0 && !GameData::Loading)
			{
				m_fade.SetFade(false);
				GameData::LoadGameData();
				NextScene.scene = NULL;
				NextScene.sceneType = ST_World;
				NextScene.id = GameData::Flags.at("map");
			}
		}
	}

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

	//if (m_fade.IsDone())
	m_battle.Update();
	//else
		//m_battle._hud.Update();

	for (auto it : m_celist)
		it.second->Physics()->DesiredMove();

	//Update
	for (auto it : m_celist)
		it.second->Update();

	//std::cout << m_actors.at(0)->GetPosRef().Print() << std::endl;
	for (auto a : m_actors)
		a->Update();

	//std::cout << "2 -- " << m_party.at(0)->_Graphics->GetPosRef().Print() << std::endl;
	m_mapHandler->Update(OrthoProjInfo::GetRegularInstance().changed);
	//std::cout << m_mapHandler->Graphics()->GetPosRef().Print() << std::endl;

	SetAudioPosition();
	SoundManager::GetInstance().Update();

	static int counter = 0;
	counter++;
	//m_camera._scale = Vector3f(sinf(counter * 0.01f) / 2 + 1, sinf(counter * 0.01) / 2 + 1, 1);
	//m_camera.MapCenter();

	//Display FPS
#ifdef _DEBUG
	static double fps = 0;
	unsigned int curfps = ElapsedTime::GetInstance().GetFPS();
	if (fps != curfps)
	{

		FontManager::GetInstance().SetText(m_fontFPS, /*std::to_string(m_celist.at(1)->PhysicsRaw()->PositionRef().z),*/std::to_string(curfps),
			Vector3f(0, OrthoProjInfo::GetRegularInstance().Top * 2.f - 0.5f * 64.0f, -10));

		fps = curfps;
	}
	//FontManager::GetInstance().SetText(m_fontFPS, std::to_string(ElapsedTime::GetInstance().GetFPS()),
	//	Vector3f(0, OrthoProjInfo::GetRegularInstance().Top * 2.f / OrthoProjInfo::GetRegularInstance().Size - 0.5f, 0));
#endif

	//srand(clock());
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

	for (auto& a : m_actors)
		Renderer::GetInstance().Add(a);

	//m_battle.SetRender();
	m_battle.GetGraphics()->SetRender();

	// Can enable post-processing effects here
	Scene::DrawEnd();
	// can disable post-processing effects here
}
