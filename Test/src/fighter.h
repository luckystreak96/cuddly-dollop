#ifndef FIGHTER_H__
#define FIGHTER_H__

#include <string>
#include <memory>
#include <vector>
#include "statUser.h"

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
	
	virtual Damage TakeDamage(Damage& dmg);
	virtual Damage ApplyHealing(Damage& heal);
	virtual void DamageModifiers(Damage& dmg, bool critting);
	virtual void ApplyLethal();
	virtual bool RespectsTargeting(Actor_ptr ap, int tm);
	virtual void TurnStart(std::vector<Actor_ptr>& actors);
	virtual void SpecialActionCommand(Damage& dmg);
	virtual void PhysicalOffenseActionCommand(Damage& dmg);
	virtual void PhysicalDefenseActionCommand(Damage& dmg);
	virtual void MagicalOffenseActionCommand(Damage& dmg);
	virtual void MagicalDefenseActionCommand(Damage& dmg);
	void SetStatsFromCurve();

public:
	virtual int DefenseDamageModification(bool critting);

public:
	bool Dead;
	Actor_ptr Protector;
	int Team;
	std::vector<Skill_ptr> Skills;

protected:
	virtual void SetDefault();
};


#endif