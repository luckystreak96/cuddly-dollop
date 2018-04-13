#include "battleHUD.h"
#include "renderer.h"
#include "fontManager.h"

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

		// Attack prediction
		if (x->_Fighter->Team != 0)
			AddActorAttackPrediction(x);
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
	if (ap->_Fighter->Team == 0)
	{
		pos = Vector3f(0.25f + 2.5f * party, 0.135f, 0);
		party++;
	}
	else
	{
		pos = Vector3f(12.8f - 2.5f * enemies, 7.825f, 0);
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
}

void BattleHUD::SetRender()
{
	for (auto& x : _hudComponents)
		x->SetRender();

	for (auto& x : _hudBG)
		Renderer::GetInstance().Add(x);
}
