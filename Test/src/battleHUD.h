#ifndef BATTLE_HUD_H__
#define BATTLE_HUD_H__

//#include "actor.h"
#include "hudHealthBar.h"
#include "battleUnit.h"
//#include "hudTurnOrder.h"
#include <map>

class BattleHUD
{
public:
	BattleHUD();
	~BattleHUD();
	void Update();
	void SetRender();
	void Destroy();
	void ToggleTurnOrderDisplay(bool hidden);
	void ToggleDamagePredictionArrowDisplay(bool hidden);

	// Returns the HudHealthBar attached to the specified actor
	HudHealthBar* GetActorHealthBar(int id);

	void AddUnit(BattleUnit unit);
	void SetupUnit(int id);

private:
	void GenerateBackground();
	void AddActorHealthBar(int id);
	void AddActorAttackPrediction(int id);
	void AddActorAttackPredictionArrow(int id);

private:
	//std::map<int, Actor_ptr> _fighters;
	//std::vector<HudComp_ptr> _hudComponents;
	std::map<int, HudBattleUnit> _units;

	std::vector<GraphComp_ptr> _hudBG;
};

#endif // !BATTLE_HUD_H__
