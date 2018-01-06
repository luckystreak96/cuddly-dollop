#ifndef ACTOR_H__
#define ACTOR_H__

#include <string>
#include <memory>
#include <vector>
#include "playerGraphicsComponent.h"

class Skill;

class Actor;
typedef std::shared_ptr<Actor> Actor_ptr;

class Actor : public PlayerGraphicsComponent
{
public:
	Actor();
	int TakeDamage(int dmg);
	void ApplyLethal();

public:
	std::string Name;
	int Health;
	int Speed;
	bool Dead;
	int Team;
	bool Selected;
	bool ChoosingAction;
	std::vector<std::shared_ptr<Skill>> Skills;
	static inline bool ActorSpeedSort(Actor_ptr a, Actor_ptr b) { return a->Speed > b->Speed; }

protected:
	void SetDefault();
};

#endif