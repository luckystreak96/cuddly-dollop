#ifndef BATTLE_HUD_H__
#define BATTLE_HUD_H__

#include "actor.h"
#include "hudHealthBar.h"
#include "hudAttackPrediction.h"

class BattleHUD
{
public:
	BattleHUD();
	void Init(std::vector<Actor_ptr> actors);
	void Update();
	void SetRender();
	void Destroy();
	void ToggleDamagePredictionDisplay(bool display);
	void ToggleDamagePredictionArrowDisplay(bool display);

	// Returns the HudHealthBar attached to the specified actor
	HudHealthBar* GetActorHealthBar(Actor* actor);

private:
	void AddActorHealthBar(Actor_ptr ap, int& party, int& enemies);
	void AddActorAttackPrediction(Actor_ptr ap);
	void AddActorAttackPredictionArrow(Actor_ptr ap);

public:
	std::vector<Actor_ptr> _actors;
	std::vector<HudComp_ptr> _hudComponents;

	std::vector<GraphComp_ptr> _hudBG;
};

#endif // !BATTLE_HUD_H__
