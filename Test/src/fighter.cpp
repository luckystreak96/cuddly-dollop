#include "fighter.h"

#include "skill.h"
#include "skillSmack.h"
#include "actor.h"
#include "statCurve.h"
#include "passiveFactory.h"
#include "battleManager.h"

Fighter::Fighter()
{
	SetDefault();
}

bool Fighter::PredictNextSkill(Actor_ptr owner, std::vector<Actor_ptr>* actors)
{
	bool done = false;
	int targ = 0;
	std::set<int> alreadyTriedSkills;
	Skill_ptr selectedSkill;

	// Ensure possible targets and skills
	while (!done)
	{
		std::set<int> alreadyTargeted;
		targ = 0;

		int skill;
		do {
			// None of the skills found valid targets -> end turn
			if (alreadyTriedSkills.size() == owner->_Fighter->Skills.size())
			{
				done = true;
				//_state = BS_ActionDone;
				PredictedSkill = NULL;
				return false;
			}
			skill = rand() % owner->_Fighter->Skills.size();
			selectedSkill = owner->_Fighter->Skills.at(skill);
		} while (alreadyTriedSkills.count(skill));
		alreadyTriedSkills.emplace(skill); // havent tried this, lets go

		do {
			// if you already tried all the actors, select another skill
			if (alreadyTargeted.size() == actors->size())
			{
				targ = -1;
				break;
			}
			targ = rand() % actors->size();
			// if the target is illegal, re-pick and dont choose that target again
			if (!actors->at(targ)->_Fighter->RespectsTargeting(owner.get(), selectedSkill->_targetMode) || // doesnt respect targeting
				actors->at(BattleManager::DefaultTargetActorIndex(actors, owner, selectedSkill))->_Fighter->Team != actors->at(targ)->_Fighter->Team)
				alreadyTargeted.emplace(targ);
		} while (targ < 0 || targ >= actors->size() || alreadyTargeted.count(targ)); // is targeting someone of a different team than the default target

		alreadyTargeted.emplace(targ);

		// target is good, can move on, otherwise choose new skill
		if (targ != -1)
			done = true;

		selectedSkill->_owner = owner;
		selectedSkill->_preCalculatedDamage = selectedSkill->CalculateDamage();
	}

	selectedSkill->_targets.clear();
	selectedSkill->_targets.push_back(actors->at(targ));
	PredictedSkill = selectedSkill;
	PredictedSkill->_isPreCalculated = true;

	// Tell the UI to update damage prediction
	UpdateObservers();

	return true;
}


void Fighter::GiveExp(int xp)
{
	Exp += xp;
	while (Exp >= NextLevelExp)
		LevelUp();
	UpdateObservers();
}

void Fighter::SetExp(int xp)
{
	Exp = xp;
	while (Exp >= NextLevelExp)
		LevelUp();
	UpdateObservers();
}

void Fighter::LevelUp()
{
	StatUser::LevelUp();
	int hp = MaxHealth.Base;
	SetStatsFromCurve();
	PassiveFactory::ApplyAllPassives(this, &_Passives);
	int dif = MaxHealth.Base - hp;
	Health += dif;
}

void Fighter::SetStatsFromCurve()
{
	StatCurve::SetCurve(this);
}

void Fighter::SetDefault()
{
	Skills.push_back(Skill_ptr(new SkillSmack()));
	Dead = false;
	Targetable = true;
	Team = 0;
	NoPredictCountDown = 1;
	Protector = NULL;
	SetStatsFromCurve();
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
	dmg._value -= Defense.Real / 2;
}

void Fighter::MagicalOffenseActionCommand(Damage& dmg)
{
	dmg._value += (dmg._value > 0) ? Strength.Real / 2 : -Strength.Real / 2;
}

void Fighter::PhysicalDefenseActionCommand(Damage& dmg)
{
	dmg._value -= Defense.Real / 2 + fmax(Level / 2, 1);
}

void Fighter::PhysicalOffenseActionCommand(Damage& dmg)
{
	dmg._value += Strength.Real / 2 + Level;
}

void Fighter::SpecialActionCommand(Damage& dmg)
{
	dmg._value = 0;
}


int Fighter::DefenseDamageModification(bool critting)
{
	// When you crit, it's like the other dude has half defense
	int def = Defense.Real;
	if (critting)
		def /= 2;

	int result = def;// *(1.0f + ((0.0006f * pow((float)def, 3.0f)) / 100.0f));
	return result;
}

void Fighter::DamageModifiers(Damage& dmg, bool critting)
{
	// Insert status effect damage changes here
	dmg._value -= DefenseDamageModification(critting);
	dmg._value = fmax(1, dmg._value);
}

Damage Fighter::TakeDamage(Damage& dmg)
{
	// Damage modifiers called in the attack method instead
	//DamageModifiers(dmg);
	dmg._value = fmax(0, dmg._value);
	Health -= dmg._value;
	Health = fmin(MaxHealth.Real, Health);
	Health = fmax(Health, 0);
	ApplyLethal();
	UpdateObservers();

	return dmg;
}

Damage Fighter::ApplyHealing(Damage& heal)
{
	// Damage modifiers called in the attack method instead
	//DamageModifiers(dmg);
	Health += heal._value;
	Health = fmin(MaxHealth.Real, Health);
	Health = fmax(Health, 0);
	ApplyLethal();
	UpdateObservers();

	return heal;
}

bool Fighter::RespectsTargeting(Actor* owner, int tm)
{
	switch (tm)
	{
	case TM_Alive:
		return !Dead;
	case TM_Ally:
		return Team == owner->_Fighter->Team;
	case TM_Any:
		return true;
	case TM_Dead:
		return Dead;
	case TM_Enemy:
		return Team != owner->_Fighter->Team;
	}

	return true;
}
