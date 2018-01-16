#ifndef BATTLE_HUD_H__
#define BATTLE_HUD_H__

#include "actor.h"

class BattleHUD
{
public:
	BattleHUD();
	void Init(std::vector<Actor_ptr> actors);
	void Update();
	void SetRender();
	
public:
	std::vector<Actor_ptr> _actors;
	std::vector<GraphComp_ptr> _actorInfo;
};

#endif // !BATTLE_HUD_H__
