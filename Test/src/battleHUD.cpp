#include "battleHUD.h"
#include "renderer.h"
#include "fontManager.h"
#include "hudArrow.h"

BattleHUD::BattleHUD()
{
}

void BattleHUD::Destroy()
{
	for (auto& x : _hudComponents)
		x->Destroy();
	_hudComponents.clear();

	_hudBG.clear();
}

void BattleHUD::Init(std::map<int, std::pair<Fighter_ptr, Actor_ptr>> map)
{
	_actors = map;

	int enemies = 0;
	int party = 0;
	for (auto& x : _actors)
	{
		// Health bar
		AddActorHealthBar(x.second, party, enemies);

		//if (x->_Fighter->Team != 0)
		{
			// Attack prediction
			AddActorAttackPrediction(x.second);

			// Attack prediction arrow
			AddActorAttackPredictionArrow(x.second);
		}
	}

	float top = OrthoProjInfo::GetRegularInstance().Top;
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float size = OrthoProjInfo::GetRegularInstance().Size;

	// Setup hudBackgrounds
	GraphComp_ptr topBG = GraphComp_ptr(new FontGraphicsComponent("TILE", "res/sprites/special/dialogue.png"));
	dynamic_cast<FontGraphicsComponent*>(topBG.get())->SetStatic(true);
	topBG->SetPhysics(Vector3f(right * 0.68f / size, top * 2 / size - 0.75f, 0.5f));
	topBG->GetModelMat()->SetScale(right * 2 / size, 0.75f, 1);
	topBG->SetColorAll(Vector3f(1, 1, 0.5f), 1.1f);
	topBG->Update();

	GraphComp_ptr bottomBG = GraphComp_ptr(new FontGraphicsComponent("TILE", "res/sprites/special/dialogue.png"));
	dynamic_cast<FontGraphicsComponent*>(bottomBG.get())->SetStatic(true);
	bottomBG->SetPhysics(Vector3f(0, 0.0f, 0.5f));
	bottomBG->GetModelMat()->SetScale(right * 1.35f / size, 0.75f, 1);
	bottomBG->SetColorAll(Vector3f(1, 1, 0.5f), 1.1f);
	bottomBG->Update();

	_hudBG.push_back(topBG);
	_hudBG.push_back(bottomBG);
}

void BattleHUD::Init(std::vector<Fighter_ptr> fighters, std::map<int, Actor_ptr> actors)
{
	_actors = std::map<int, std::pair<Fighter_ptr, Actor_ptr>>();
	for (auto& x : fighters)
		_actors.emplace(x->_BattleFieldPosition, std::pair<Fighter_ptr, Actor_ptr>(x, actors.at(x->_BattleFieldPosition)));

	Init(_actors);
}


void BattleHUD::AddActorHealthBar(std::pair<Fighter_ptr, Actor_ptr> ap, int& party, int& enemies)
{
	Vector3f pos;
	int right = (int)OrthoProjInfo::GetRegularInstance().Right * 2;
	int up = (int)OrthoProjInfo::GetRegularInstance().Top * 2;
	int size = (int)OrthoProjInfo::GetRegularInstance().Size;
	float orthoWidth = (float)right / (float)size;
	float orthoHeight = (float)up / (float)size;
	std::cout << orthoWidth << std::endl;
	if (ap.first->Team == 0)
	{
		float xpos1 = (0.25f / 15.f) * orthoWidth;
		float xpos2 = (2.5f / 15.f) * orthoWidth;
		float ypos = (0.135f / 8.4375f) * orthoHeight;
		pos = Vector3f(xpos1 + xpos2 * party, ypos, 0);
		party++;
	}
	else
	{
		float xpos1 = (12.8f / 15.f) * orthoWidth;
		float xpos2 = (2.5f / 15.f) * orthoWidth;
		float ypos = 7.825f / 8.4375f;
		pos = Vector3f(xpos1 - xpos2 * enemies, ypos * orthoHeight, 0);
		enemies++;
	}

	HudComp_ptr healthBar = std::make_shared<HudHealthBar>(HudHealthBar(ap, pos));
	ap.first->_observers.push_back(healthBar);
	ap.second->_observers.push_back(healthBar);
	_hudComponents.push_back(healthBar);
}

void BattleHUD::AddActorAttackPrediction(std::pair<Fighter_ptr, Actor_ptr> ap)
{
	HudComp_ptr damagePrediction = std::make_shared<HudTurnOrder>(HudTurnOrder(ap));
	ap.first->_observers.push_back(damagePrediction);
	ap.second->_observers.push_back(damagePrediction);
	_hudComponents.push_back(damagePrediction);
}

void BattleHUD::AddActorAttackPredictionArrow(std::pair<Fighter_ptr, Actor_ptr> fa)
{
	HudComp_ptr arrow = HudComp_ptr(new HudArrow(fa, &_actors));
	fa.first->_observers.push_back(arrow);
	_hudComponents.push_back(arrow);
}

void BattleHUD::ToggleTurnOrderDisplay(bool hidden)
{
	HudTurnOrder* pred;
	for (auto& x : _hudComponents)
	{
		pred = dynamic_cast<HudTurnOrder*>(x.get());
		if (pred != NULL)
		{
			pred->ToggleDisplay(hidden);
		}
	}
}

void BattleHUD::ToggleDamagePredictionArrowDisplay(bool hidden)
{
	for (auto& x : _actors)
		if (x.second.first->Team == 0 && x.second.first->NoPredictCountDown > 0)
			hidden = true;

	HudArrow* pred;
	for (auto& x : _hudComponents)
	{
		pred = dynamic_cast<HudArrow*>(x.get());
		if (pred != NULL)
		{
			//if (pred->_targeter->_Fighter->Team == 0 && hidden)
			//	continue;
			pred->ToggleHidden(hidden);
		}
	}
}

HudHealthBar* BattleHUD::GetActorHealthBar(Fighter_ptr fighter)
{
	HudHealthBar* bar;
	for (auto& x : _hudComponents)
	{
		bar = dynamic_cast<HudHealthBar*>(x.get());
		if (bar != NULL && bar->_actor.first == fighter)
			return bar;
	}
}

void BattleHUD::Update()
{
	if (OrthoProjInfo::GetRegularInstance().changed)
	{
		Destroy();
		Init(_actors);
	}

	// Update target arrows
	//for (auto& x : _actors)
	//{
	//	if (!x->_Fighter->Dead && x->_Fighter->PredictedSkill != NULL)
	//	{

	//	}
	//}
}

void BattleHUD::SetRender()
{
	for (auto& x : _hudComponents)
		x->SetRender();

	for (auto& x : _hudBG)
		Renderer::GetInstance().Add(x);
}
