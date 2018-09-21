#ifndef BATTLE_HUD_H__
#define BATTLE_HUD_H__

#include "actor.h"
#include "hudHealthBar.h"
#include "hudTurnOrder.h"

class BattleHUD
{
public:
	BattleHUD();
	void Init(std::vector<Fighter_ptr> fighters, std::map<int, Actor_ptr> actors);
	void Init(std::map<int, std::pair<Fighter_ptr, Actor_ptr>> map);
	void Update();
	void SetRender();
	void Destroy();
	void ToggleTurnOrderDisplay(bool hidden);
	void ToggleDamagePredictionArrowDisplay(bool hidden);

	// Returns the HudHealthBar attached to the specified actor
	HudHealthBar* GetActorHealthBar(Fighter_ptr fighter);

private:
	void AddActorHealthBar(std::pair<Fighter_ptr, Actor_ptr> ap, int& party, int& enemies);
	void AddActorAttackPrediction(std::pair<Fighter_ptr, Actor_ptr> ap);
	void AddActorAttackPredictionArrow(std::pair<Fighter_ptr, Actor_ptr>);

public:
	std::map<int, std::pair<Fighter_ptr, Actor_ptr>> _actors;
	std::vector<HudComp_ptr> _hudComponents;

	std::vector<GraphComp_ptr> _hudBG;
};

#endif // !BATTLE_HUD_H__
