#ifndef BATTLE_HUD_H__
#define BATTLE_HUD_H__

#include "actor.h"

enum HUDComponent { HC_HealthBar, HC_Text };

struct ActorInfoBundle
{
	HUDComponent _type;
	GraphComp_ptr _foreground;
	GraphComp_ptr _background;
	unsigned int _health;
	unsigned int _name;
};

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
	std::vector<ActorInfoBundle> _actorInfo;
};

#endif // !BATTLE_HUD_H__
