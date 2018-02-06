#include "actor.h"
#include "skill.h"
#include "skillSmack.h"

Actor::Actor()
{
	SetDefault();
}

void Actor::SetDefault()
{
	Skills.push_back(Skill_ptr(new SkillSmack()));
	Skills.push_back(Skill_ptr(new Skill()));
	SetTexture("res/sprites/entities/entity_ghost.png");
	Name = "Player";
	Health = 10;
	MaxHealth = 10;
	Dead = false;
	Speed = 0;
	Team = 0;
	Selected = false;
	ChoosingAction = false;
	Protector = NULL;
}

void Actor::TurnStart(std::vector<Actor_ptr>& actors)
{
	for (auto a : actors)
	{
		if (a->Protector.get() == this)
			a->Protector = NULL;
	}
}

void Actor::ApplyLethal()
{
	Dead = Health <= 0;
	SetColor();
}

void Actor::DefenseActionCommand(int& dmg)
{
	dmg -= 3;
}

void Actor::SetColor()
{
	if (ChoosingAction)
		SetColorAll(/*Vector3f(1.f, 1.f, 0), 1.f*/);
	else if (Dead)
		SetColorAll(Vector3f(0.25f, 0.25f, 0.25f), 0.5f);
	else
		SetColorAll();
}

int Actor::TakeDamage(int dmg)
{
	// Apply damage reduction here
	Health -= dmg;
	Health = fmin(MaxHealth, Health);
	Health = fmax(Health, 0);
	ApplyLethal();

	return dmg;
}

bool Actor::RespectsTargeting(Actor_ptr ap, int tm)
{
	switch (tm)
	{
	case TM_Alive:
		return !Dead;
	case TM_Ally:
		return Team == ap->Team;
	case TM_Any:
		return true;
	case TM_Dead:
		return Dead;
	case TM_Enemy:
		return Team != ap->Team;
	}

	return true;
}


void Actor::Update()
{
	PlayerGraphicsComponent::Update();
}
