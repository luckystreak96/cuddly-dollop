#include "battleHUD.h"
#include "renderer.h"
#include "fontManager.h"
#include "hudArrow.h"
#include "hudTurnOrder.h"
#include "hudStatusEffect.h"

BattleHUD::BattleHUD()
{
	GenerateBackground();
}

BattleHUD::~BattleHUD()
{
	Destroy();
}

void BattleHUD::Destroy()
{
	for (auto& x : _units)
	{
		for (auto& y : x.second.hud)
		{
			y->Destroy();
		}
		x.second.hud.clear();
	}
	//_hudComponents.clear();

	_hudBG.clear();
}

void BattleHUD::AddUnit(BattleUnit unit)
{
	HudBattleUnit hudUnit;
	hudUnit.unit = unit;
	_units.emplace(unit.id, hudUnit);

	SetupUnit(unit.id);
}

void BattleHUD::SetupUnit(int id)
{
	// Health bar
	AddActorHealthBar(id);

	// Attack prediction
	AddActorAttackPrediction(id);

	// Status hud
	AddActorStatus(id);

	// Attack prediction arrow
	// Slight problem - sometimes an enemy can target an ally that hasnt been added yet
	//AddActorAttackPredictionArrow(id);
}



void BattleHUD::GenerateBackground()
{
	float top = OrthoProjInfo::GetRegularInstance().Top;
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float size = OrthoProjInfo::GetRegularInstance().Size;

	// Setup hudBackgrounds
	std::shared_ptr<GraphicsComponent> topBG = std::shared_ptr<GraphicsComponent>(new FontGraphicsComponent("TILE", "res/sprites/special/dialogue.png"));
	dynamic_cast<FontGraphicsComponent*>(topBG.get())->SetStatic(true);
	topBG->set_position(Vector3f(right * 0.68f, top * 2 - 0.75f * size, 0.5f));
	topBG->GetModelMat()->SetScale(right * 2, 0.75f, 1);
	topBG->SetColorAll(Vector3f(1, 1, 0.5f), 1.1f);
	topBG->Update();

	std::shared_ptr<GraphicsComponent> bottomBG = std::shared_ptr<GraphicsComponent>(new FontGraphicsComponent("TILE", "res/sprites/special/dialogue.png"));
	dynamic_cast<FontGraphicsComponent*>(bottomBG.get())->SetStatic(true);
	bottomBG->set_position(Vector3f(0, 0.0f, 0.5f));
	bottomBG->GetModelMat()->SetScale(right * 1.35f, 0.75f, 1);
	bottomBG->SetColorAll(Vector3f(1, 1, 0.5f), 1.1f);
	bottomBG->Update();

	_hudBG.push_back(topBG);
	_hudBG.push_back(bottomBG);
}

void BattleHUD::AddActorHealthBar(int id)
{
	Vector3f pos;
	float right = OrthoProjInfo::GetRegularInstance().Right * 2.f;
	float up = OrthoProjInfo::GetRegularInstance().Top * 2.f;
	float size = OrthoProjInfo::GetRegularInstance().Size;
	float orthoWidth = right / size;
	float orthoHeight = up / size;

	HudBattleUnit& u = _units.at(id);

	float barOffsets = (2.5f / 15.f) * orthoWidth;

	if (u.unit.team == 0)
	{
		float xpos1 = (0.25f / 15.f) * orthoWidth;
		float ypos = (0.1f / 8.4375f) * orthoHeight;
		pos = Vector3f(xpos1 + (barOffsets * (3 - (u.unit.id % 4))) /*+ barOffsets * (u.unit.id % 4)*/, ypos, 0);
	}
	else
	{
		float xpos1 = (12.8f / 15.f) * orthoWidth;
		float ypos = 8.0f / 8.4375f;
		pos = Vector3f(xpos1 - barOffsets * (u.unit.id % 4), ypos * orthoHeight, 0);
	}

	HudComp_ptr healthBar = std::shared_ptr<HudHealthBar>(new HudHealthBar(u.unit, pos));
	u.unit.fobservers->push_back(healthBar);
	u.unit.aobservers->push_back(healthBar);
	u.hud.push_back(healthBar);
}

void BattleHUD::AddActorStatus(int id)
{
	BattleUnit& u = _units.at(id).unit;
	HudComp_ptr statusHud = std::shared_ptr<HudStatusEffect>(new HudStatusEffect(u));

	u.aobservers->push_back(statusHud);
	u.fobservers->push_back(statusHud);
	_units.at(id).hud.push_back(statusHud);
}

void BattleHUD::AddActorAttackPrediction(int id)
{
	BattleUnit& u = _units.at(id).unit;
	HudComp_ptr damagePrediction = std::shared_ptr<HudTurnOrder>(new HudTurnOrder(u));
	u.aobservers->push_back(damagePrediction);
	u.fobservers->push_back(damagePrediction);
	_units.at(id).hud.push_back(damagePrediction);
}

void BattleHUD::AddActorAttackPredictionArrow(int id)
{
	HudComp_ptr arrow = HudComp_ptr(new HudArrow(_units.at(id).unit, &_units));
	_units.at(id).unit.fobservers->push_back(arrow);
	_units.at(id).hud.push_back(arrow);
}

void BattleHUD::ToggleTurnOrderDisplay(bool hidden)
{
	HudTurnOrder* pred;
	for (auto& y : _units)
		for (auto& x : y.second.hud)
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
	for (auto& x : _units)
		if (x.second.unit.team == 0 /*&& x.second.first->NoPredictCountDown > 0*/)
			hidden = true;

	HudArrow* pred;
	for (auto& y : _units)
		for (auto& x : y.second.hud)
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

HudHealthBar* BattleHUD::GetActorHealthBar(int id)
{
	HudHealthBar* bar;
	auto& y = _units.at(id).hud;
	for (auto& x : y)
	{
		bar = dynamic_cast<HudHealthBar*>(x.get());
		if (bar != NULL)
			return bar;
	}

	return nullptr;
}

void BattleHUD::Update()
{
	if (OrthoProjInfo::GetRegularInstance().changed)
	{
		Destroy();
		GenerateBackground();
		for (auto& x : _units)
			SetupUnit(x.first);
	}

	// Update target arrows
	//for (auto& x : _fighters)
	//{
	//	if (!x->_Fighter->Dead && x->_Fighter->PredictedSkill != NULL)
	//	{

	//	}
	//}
}

void BattleHUD::SetRender()
{
	for (auto& y : _units)
		for (auto& x : y.second.hud)
			x->SetRender();

	for (auto& x : _hudBG)
		Renderer::GetInstance().Add(x);
}
