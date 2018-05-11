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

void BattleHUD::Init(std::vector<Actor_ptr> actors)
{
	_actors = actors;

	int enemies = 0;
	int party = 0;
	for (auto& x : _actors)
	{
		// Health bar
		AddActorHealthBar(x, party, enemies);

		if (x->_Fighter->Team != 0)
		{
			// Attack prediction
			AddActorAttackPrediction(x);

			// Attack prediction arrow
			AddActorAttackPredictionArrow(x);
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


void BattleHUD::AddActorHealthBar(Actor_ptr ap, int& party, int& enemies)
{
	Vector3f pos;
	int right = OrthoProjInfo::GetRegularInstance().Right * 2;
	int up = OrthoProjInfo::GetRegularInstance().Top * 2;
	int size = OrthoProjInfo::GetRegularInstance().Size;
	float orthoWidth = (float)right / (float)size;
	float orthoHeight = (float)up / (float)size;
	std::cout << orthoWidth << std::endl;
	if (ap->_Fighter->Team == 0)
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

	HudComp_ptr healthBar = std::make_shared<HudHealthBar>(HudHealthBar(ap.get(), pos));
	ap->_Fighter->_observers.push_back(healthBar);
	ap->_Graphics->_observers.push_back(healthBar);
	_hudComponents.push_back(healthBar);
}

void BattleHUD::AddActorAttackPrediction(Actor_ptr ap)
{
	HudComp_ptr damagePrediction = std::make_shared<HudAttackPrediction>(HudAttackPrediction(ap.get()));
	ap->_Fighter->_observers.push_back(damagePrediction);
	ap->_Graphics->_observers.push_back(damagePrediction);
	_hudComponents.push_back(damagePrediction);
}

void BattleHUD::AddActorAttackPredictionArrow(Actor_ptr ap)
{
	HudComp_ptr damagePrediction = HudComp_ptr(new HudArrow(ap));
	ap->_Fighter->_observers.push_back(damagePrediction);
	_hudComponents.push_back(damagePrediction);
}

void BattleHUD::ToggleDamagePredictionDisplay(bool display)
{
	HudAttackPrediction* pred;
	for (auto& x : _hudComponents)
	{
		pred = dynamic_cast<HudAttackPrediction*>(x.get());
		if (pred != NULL)
			pred->ToggleDisplay(display);
	}
}

void BattleHUD::ToggleDamagePredictionArrowDisplay(bool display)
{
	HudArrow* pred;
	for (auto& x : _hudComponents)
	{
		pred = dynamic_cast<HudArrow*>(x.get());
		if (pred != NULL)
			pred->ToggleHidden(display);
	}
}

HudHealthBar* BattleHUD::GetActorHealthBar(Actor* actor)
{
	HudHealthBar* bar;
	for (auto& x : _hudComponents)
	{
		bar = dynamic_cast<HudHealthBar*>(x.get());
		if (bar != NULL && bar->_actor == actor)
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
