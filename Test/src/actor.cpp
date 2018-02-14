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
	//Skills.push_back(Skill_ptr(new Skill()));
	Sprite = "res/sprites/entities/entity_ghost.png";
	SetTexture(Sprite);
	Name = "Player";
	Health = 10;
	MaxHealth = 10;
	Dead = false;
	Speed = 0;
	Strength = 0;
	Defense = 0;
	Endurance = 0;
	Crit = 0;
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

void Actor::MagicalDefenseActionCommand(int& dmg)
{
	dmg -= Defense / 2;
}

void Actor::MagicalOffenseActionCommand(int& dmg)
{
	dmg += (dmg > 0) ? Strength / 2 : -Strength / 2;
}

void Actor::PhysicalDefenseActionCommand(int& dmg)
{
	dmg -= Defense / 2;
}

void Actor::PhysicalOffenseActionCommand(int& dmg)
{
	dmg += Strength / 2;
}

void Actor::SpecialActionCommand(int& dmg)
{
	dmg = 0;
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

void Actor::SetEndurance(int endurance)
{
	SetMaxHealth(5 + endurance * 3 * (1.0f + (float)endurance / 100.0f));
}

void Actor::SetMaxHealth(int max)
{
	if (Health > max)
		Health = max;

	MaxHealth = max;
}

int Actor::DefenseDamageModification(bool critting)
{
	// When you crit, it's like the other dude has half defense
	int def = Defense;
	if (critting)
		def /= 2;

	int result = def * (1.0f + ((0.0006f * pow((float)def, 3.0f)) / 100.0f));
	return result;
}

void Actor::DamageModifiers(int& dmg, bool critting)
{
	// Insert status effect damage changes here
	dmg -= DefenseDamageModification(critting);
}

int Actor::TakeDamage(int& dmg)
{
	// Damage modifiers called in the attack method instead
	//DamageModifiers(dmg);
	dmg = fmax(0, dmg);
	Health -= dmg;
	Health = fmin(MaxHealth, Health);
	Health = fmax(Health, 0);
	ApplyLethal();

	return dmg;
}

int Actor::ApplyHealing(int& heal)
{
	// Damage modifiers called in the attack method instead
	//DamageModifiers(dmg);
	Health += heal;
	Health = fmin(MaxHealth, Health);
	Health = fmax(Health, 0);
	ApplyLethal();

	return heal;
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

int Actor::GetEndurance()
{
	return Endurance;
}

int Actor::GetMaxHealth()
{
	return MaxHealth;
}
