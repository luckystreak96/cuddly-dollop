#ifndef FIGHTER_H__
#define FIGHTER_H__

#include <string>
#include <memory>
#include <vector>
#include "statUser.h"
#include "passiveSkill.h"

class Skill;
typedef std::shared_ptr<Skill> Skill_ptr;

class Fighter;
typedef std::shared_ptr<Fighter> Fighter_ptr;

class Actor;
typedef std::shared_ptr<Actor> Actor_ptr;

struct Damage;

class Fighter : public StatUser
{
public:
	Fighter();
	
	// Damage related
	virtual Damage TakeDamage(Damage& dmg);
	virtual Damage ApplyHealing(Damage& heal);
	virtual void DamageModifiers(Damage& dmg, bool critting);
	virtual void ApplyLethal();

	// Turn and targets
	virtual bool RespectsTargeting(Actor* ap, int tm);
	virtual void TurnStart(std::vector<Actor_ptr>& actors);
	// Returns whether a skill could be chosen or not
	virtual bool PredictNextSkill(Actor_ptr owner, std::vector<Actor_ptr>* actors);

	// Action Commands
	virtual void SpecialActionCommand(Damage& dmg);
	virtual void PhysicalOffenseActionCommand(Damage& dmg);
	virtual void PhysicalDefenseActionCommand(Damage& dmg);
	virtual void MagicalOffenseActionCommand(Damage& dmg);
	virtual void MagicalDefenseActionCommand(Damage& dmg);

	// Levels
	virtual void LevelUp();
	virtual void GiveExp(int xp);
	virtual void SetExp(int xp);

	// Stats
	void SetStatsFromCurve();

public:
	virtual int DefenseDamageModification(bool critting);

public:
	std::vector<Passive_ptr> _Passives;
	bool Dead;
	Actor_ptr Protector;
	int Team;
	std::vector<Skill_ptr> Skills;
	Skill_ptr PredictedSkill;

protected:
	virtual void SetDefault();
};


#endif