#include "actor.h"
#include "skill.h"
#include "skillSmack.h"

Actor::Actor()
{
	SetDefault();
}

void Actor::SetDefault()
{
	Skills.push_back(std::shared_ptr<Skill>(new SkillSmack()));
	Skills.push_back(std::shared_ptr<Skill>(new Skill()));
	SetTexture("res/sprites/entities/entity_ghost.png");
	Name = "Player";
	Health = 10;
	Dead = false;
	Speed = 0;
	Team = 0;
	Selected = false;
	ChoosingAction = false;
}

void Actor::ApplyLethal()
{
	Dead = Health <= 0;
}

int Actor::TakeDamage(int dmg)
{
	// Apply damage reduction here
	Health -= dmg;
	ApplyLethal();

	return dmg;
}
