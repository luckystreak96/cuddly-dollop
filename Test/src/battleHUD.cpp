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
}

void BattleHUD::Init(std::vector<Actor_ptr> actors)
{
	_actors = actors;

	int enemies = 0;
	int party = 0;
	for (auto& x : _actors)
	{
		// HEALTH BAR
		AddActorHealthBar(x, party, enemies);
	}
}


void BattleHUD::AddActorHealthBar(Actor_ptr ap, int& party, int& enemies)
{
	Vector3f pos;
	if (ap->_Fighter->Team == 0)
	{
		pos = Vector3f(2.75f + 2.5f * party, 0.5f, 0);
		party++;
	}
	else
	{
		pos = Vector3f(2.75f + 2.5f * enemies, 7.30f, 0);
		enemies++;
	}

	HudComp_ptr healthBar = std::make_shared<HudHealthBar>(HudHealthBar(&ap->_Fighter->Health, ap->_Fighter->GetMaxHealth().Real, pos, ap->_Name));
	ap->_Fighter->_observers.push_back(healthBar);
	_hudComponents.push_back(healthBar);
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
}
