#include "fighter.h"

#include "skill.h"
#include "skillSmack.h"
#include "actor.h"
#include "statCurve.h"

Fighter::Fighter()
{
	SetDefault();
}

void Fighter::SetStatsFromCurve()
{
	// fighter needs to somehow load his statcurve from file
	StatCurve::SetCurve(0, this);
}

void Fighter::SetDefault()
{
	Skills.push_back(Skill_ptr(new SkillSmack()));
	Dead = false;
	Team = 0;
	Protector = NULL;
}

void Fighter::TurnStart(std::vector<Actor_ptr>& actors)
{
	for (auto a : actors)
	{
		if (a->_Fighter->Protector && a->_Fighter->Protector->_Fighter.get() == this)
			a->_Fighter->Protector = NULL;
	}
}

void Fighter::ApplyLethal()
{
	Dead = Health <= 0;
	//UpdateColor();
}

void Fighter::MagicalDefenseActionCommand(Damage& dmg)
{
	dmg._value -= Defense / 2;
}

void Fighter::MagicalOffenseActionCommand(Damage& dmg)
{
	dmg._value += (dmg._value > 0) ? Strength / 2 : -Strength / 2;
}

void Fighter::PhysicalDefenseActionCommand(Damage& dmg)
{
	dmg._value -= Defense / 2;
}

void Fighter::PhysicalOffenseActionCommand(Damage& dmg)
{
	dmg._value += Strength / 2;
}

void Fighter::SpecialActionCommand(Damage& dmg)
{
	dmg._value = 0;
}


int Fighter::DefenseDamageModification(bool critting)
{
	// When you crit, it's like the other dude has half defense
	int def = Defense;
	if (critting)
		def /= 2;

	int result = def * (1.0f + ((0.0006f * pow((float)def, 3.0f)) / 100.0f));
	return result;
}

void Fighter::DamageModifiers(Damage& dmg, bool critting)
{
	// Insert status effect damage changes here
	dmg._value -= DefenseDamageModification(critting);
}

Damage Fighter::TakeDamage(Damage& dmg)
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

Damage Fighter::ApplyHealing(Damage& heal)
{
	// Damage modifiers called in the attack method instead
	//DamageModifiers(dmg);
	Health += heal._value;
	Health = fmin(MaxHealth, Health);
	Health = fmax(Health, 0);
	ApplyLethal();

	return heal;
}

bool Fighter::RespectsTargeting(Actor_ptr ap, int tm)
{
	switch (tm)
	{
	case TM_Alive:
		return !Dead;
	case TM_Ally:
		return Team == ap->_Fighter->Team;
	case TM_Any:
		return true;
	case TM_Dead:
		return Dead;
	case TM_Enemy:
		return Team != ap->_Fighter->Team;
	}

	return true;
}
