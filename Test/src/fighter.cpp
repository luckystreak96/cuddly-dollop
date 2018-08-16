#include "fighter.h"

#include "skill.h"
#include "skillSmack.h"
#include "actor.h"
#include "statCurve.h"
#include "passiveFactory.h"
#include "battleManager.h"
#include "actorFactory.h"

Fighter::Fighter()
{
	SetDefault();
}

Fighter::Fighter(Fighter& f)
{
	SetDefault();

	Skills.clear();
	Health = f.Health;
	Curve = f.Curve;
	SetLevel(f.Level);
	SetExp(f.Exp);
	SkillPoints = f.SkillPoints;

	Dead = f.Dead;

	for (auto& x : f.Skills)
		Skills.push_back(ActorFactory::BuildSkill(x->_name));

	for (auto& x : f._Passives)
		_Passives.push_back(BattleData::PassiveSkills.at(x->_Id));

	ReCalculateStats();
}

int Fighter::HasElement(SkillElement element)
{
	int result = 0;
	for (auto& x : _Statuses)
		if (x._Element == element)
			result++;

	return result;
}

void Fighter::ReCalculateStats()
{
	// Stats are decided first by the curve, then the passives, then the equipment etc
	SetStatsFromCurve();
	PassiveFactory::ApplyAllPassives(this, &_Passives);

	std::vector<Passive_ptr> statuses;
	for (auto& x : _Statuses)
	{
		for (auto y : x._Effects)
		{
			for (int i = 0; i < std::get<1>(y); i++)
				statuses.push_back(std::get<0>(y));
		}
	}

	PassiveFactory::ApplyAllPassives(this, &statuses);

	//ResetRealModified();

	CurrentHealthCheck();
}

void Fighter::SetLevel(int level)
{
	StatUser::SetLevel(level);
	ReCalculateStats();
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
				((actors->at(targ)->_Fighter->Team != owner->_Fighter->Team && selectedSkill->_defaultTarget == DT_Ally) ||
				(actors->at(targ)->_Fighter->Team == owner->_Fighter->Team && selectedSkill->_defaultTarget == DT_Enemy)))
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
	ReCalculateStats();
	int dif = MaxHealth.Base - hp;
	Health += dif;
}

void Fighter::SetStatsFromCurve()
{
	StatCurve::SetCurve(this);
}

void Fighter::SetDefault()
{
	_OrderPosition = 0;
	_BattleFieldPosition = 0;
	Skills.push_back(Skill_ptr(new SkillSmack()));
	Dead = false;
	Targetable = true;
	Team = 0;
	NoPredictCountDown = 0;
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
	UpdateObservers();
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

void Fighter::ApplyBonusDamage(Actor_ptr target)
{
	PredictedSkill->ApplyBonusEffect(target);
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

void Fighter::SetOrderPosition(int pos)
{
	_OrderPosition = pos;
	UpdateObservers();
}
