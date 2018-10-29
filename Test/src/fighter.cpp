#include "fighter.h"

#include "skill.h"
#include "skillSmack.h"
#include "statCurve.h"
#include "passiveFactory.h"
#include "battleManager.h"
#include "fighterFactory.h"

Fighter::Fighter()
{
	SetDefault();
}

Fighter::Fighter(Fighter& f)
{
	SetDefault();

	m_skills.clear();
	m_sprite = f.m_sprite;
	m_name = f.m_name;
	Health = f.Health;
	Curve = f.Curve;
	SetLevel(f.Level);
	SetExp(f.Exp);
	SkillPoints = f.SkillPoints;

	Dead = f.Dead;

	for (auto& x : f.m_skills)
		m_skills.push_back(FighterFactory::BuildSkill(x->GetName()));

	for (auto& x : f._Passives)
		_Passives.push_back(BattleData::PassiveSkills.at(x->_Id));

	ReCalculateStats();
}

const std::vector<Skill_ptr>& Fighter::GetSkills() const
{
	return m_skills;
}

std::vector<Skill_ptr>& Fighter::GetSkills()
{
	return m_skills;
}


bool Fighter::FighterSpeedSort(Fighter_ptr a, Fighter_ptr b)
{
	if (a && b)
		return a->Speed.Modified > b->Speed.Modified;
	return true;
}

bool Fighter::FighterBattleOrderSort(Fighter_ptr a, Fighter_ptr b)
{
	if (a && b)
		return a->_BattleFieldPosition < b->_BattleFieldPosition;
	return true;
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

bool Fighter::PredictNextSkill(Fighter_ptr owner, std::vector<Fighter_ptr>* fighters)
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
			if (alreadyTriedSkills.size() == owner->m_skills.size())
			{
				done = true;
				//_state = BS_ActionDone;
				PredictedSkill = NULL;
				return false;
			}
			skill = rand() % owner->m_skills.size();
			selectedSkill = owner->m_skills.at(skill);
		} while (alreadyTriedSkills.count(skill));
		alreadyTriedSkills.emplace(skill); // havent tried this, lets go

		do {
			// if you already tried all the fighters, select another skill
			if (alreadyTargeted.size() == fighters->size())
			{
				targ = -1;
				break;
			}
			targ = rand() % fighters->size();
			// if the target is illegal, re-pick and dont choose that target again
			if (!fighters->at(targ)->RespectsTargeting(owner, selectedSkill->_targetMode) || // doesnt respect targeting
				((fighters->at(targ)->Team != owner->Team && selectedSkill->_defaultTarget == DT_Ally) ||
				(fighters->at(targ)->Team == owner->Team && selectedSkill->_defaultTarget == DT_Enemy)))
				alreadyTargeted.emplace(targ);
		} while (targ < 0 || targ >= fighters->size() || alreadyTargeted.count(targ)); // is targeting someone of a different team than the default target

		alreadyTargeted.emplace(targ);

		// target is good, can move on, otherwise choose new skill
		if (targ != -1)
			done = true;

		//selectedSkill->_owner = owner;
		selectedSkill->_preCalculatedDamage = selectedSkill->CalculateDamage(*this);
	}

	_targets.clear();
	_targets.push_back(fighters->at(targ)->GetId());
	PredictedSkill = selectedSkill;
	PredictedSkill->_isPreCalculated = true;

	// Tell the UI to update damage prediction
	UpdateObservers();

	return true;
}

void Fighter::SetSkills(std::vector<Skill_ptr> skills)
{
	// swap values of the vectors
	m_skills.swap(skills);
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
	//m_skills.push_back(Skill_ptr(new SkillSmack()));
	Dead = false;
	Targetable = true;
	Team = 0;
	NoPredictCountDown = 0;
	Protector = NULL;
	SetStatsFromCurve();
}

void Fighter::UseSkill()
{
	//std::sort(_targets.begin(), _targets.end(), Fighter::FighterBattleOrderSort);
}


void Fighter::TurnStart(std::vector<Fighter_ptr>& actors)
{
	for (auto a : actors)
	{
		if (a->Protector && a->Protector.get() == this)
			a->Protector = NULL;
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
	dmg._value -= Defense.Real;
}

void Fighter::MagicalOffenseActionCommand(Damage& dmg)
{
	dmg._value += (dmg._value > 0) ? Strength.Real : -Strength.Real;
}

void Fighter::PhysicalDefenseActionCommand(Damage& dmg)
{
	dmg._value -= Defense.Real + fmax(Level / 2, 1);
}

void Fighter::PhysicalOffenseActionCommand(Damage& dmg)
{
	dmg._value += Strength.Real + Level;
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
	Health = min(MaxHealth.Real, Health);
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

void Fighter::ApplyBonusDamage(Fighter_ptr target)
{
	//PredictedSkill->ApplyBonusEffect(target);
}

bool Fighter::RespectsTargeting(Fighter_ptr owner, int tm)
{
	switch (tm)
	{
	case TM_Alive:
		return !Dead;
	case TM_Ally:
		return Team == owner->Team;
	case TM_Any:
		return true;
	case TM_Dead:
		return Dead;
	case TM_Enemy:
		return Team != owner->Team;
	}

	return true;
}

void Fighter::SetOrderPosition(int pos)
{
	_OrderPosition = pos;
	UpdateObservers();
}


BattleUnit Fighter::create_battle_unit()
{
	BattleUnit unit;
	unit.name = m_name;
	unit.fobservers = &_observers;
	unit.stats = (StatUser*)&*this;
	unit.id = _BattleFieldPosition;
	unit.team = Team;
	unit.turnOrder = &_OrderPosition;
	unit.dead = &Dead;
	unit.targets = &_targets;

	return unit;
}