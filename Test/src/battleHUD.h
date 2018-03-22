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

private:
	void AddActorHealthBar(Actor_ptr ap, int& party, int& enemies);

public:
	std::vector<Actor_ptr> _actors;
	std::vector<HudComp_ptr> _hudComponents;
};

#endif // !BATTLE_HUD_H__
