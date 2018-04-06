#ifndef BATTLE_HUD_H__
#define BATTLE_HUD_H__

#include "actor.h"
#include "hudHealthBar.h"

class BattleHUD
{
public:
	BattleHUD();
	void Init(std::vector<Actor_ptr> actors);
	void Update();
	void SetRender();
	void Destroy();
	HudHealthBar* GetHudHealthBar(Actor* actor);

private:
	void AddActorHealthBar(Actor_ptr ap, int& party, int& enemies);

public:
	std::vector<Actor_ptr> _actors;
	std::vector<HudComp_ptr> _hudComponents;

	// Used to access the health bars from xp animation
	std::vector<HudHealthBar*> _hudHealthBars;

	std::vector<GraphComp_ptr> _hudBG;
};

#endif // !BATTLE_HUD_H__
