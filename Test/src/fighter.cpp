#include "fighter.h"

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
	NextLevelExp = 0;
	Level = 1;
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

void Actor::MagicalDefenseActionCommand(Damage& dmg)
{
	dmg._value -= Defense / 2;
}

void Actor::MagicalOffenseActionCommand(Damage& dmg)
{
	dmg._value += (dmg._value > 0) ? Strength / 2 : -Strength / 2;
}

void Actor::PhysicalDefenseActionCommand(Damage& dmg)
{
	dmg._value -= Defense / 2;
}

void Actor::PhysicalOffenseActionCommand(Damage& dmg)
{
	dmg._value += Strength / 2;
}

void Actor::SpecialActionCommand(Damage& dmg)
{
	dmg._value = 0;
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

void Actor::DamageModifiers(Damage& dmg, bool critting)
{
	// Insert status effect damage changes here
	dmg._value -= DefenseDamageModification(critting);
}

Damage Actor::TakeDamage(Damage& dmg)
{
	// Damage modifiers called in the attack method instead
	//DamageModifiers(dmg);
	dmg._value = fmax(0, dmg._value);
	Health -= dmg._value;
	Health = fmin(MaxHealth, Health);
	Health = fmax(Health, 0);
	ApplyLethal();

	return dmg;
}

Damage Actor::ApplyHealing(Damage& heal)
{
	// Damage modifiers called in the attack method instead
	//DamageModifiers(dmg);
	Health += heal._value;
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

void Actor::SetLevel(int level)
{
	Level = level;
	NextLevelExp = CalculateNextLevelExp();
}

void Actor::LevelUp()
{
	SkillPoints += 4;
	NextLevelExp = CalculateNextLevelExp();

	MaxHealth += 2;
	if (Level % 2 == 0)
	{
		Strength += 1;
		Defense += 1;
	}
	else
	{
		Endurance += 1;
		Speed += 1;
	}
}

int Actor::CalculateNextLevelExp()
{
	return 10 * Level + pow(Level, 2);
}

int Actor::GetLevel()
{
	return Level;
}

void Actor::GiveExp(int exp)
{
	Exp += exp;
	while (exp > NextLevelExp)
	{
		// Level up
		LevelUp();
	}
}

void Actor::SetExp(int exp)
{
	Exp = exp;
}

int Actor::GetExp()
{
	return Exp;
}

int Actor::GetMaxHealth()
{
	return MaxHealth;
}
