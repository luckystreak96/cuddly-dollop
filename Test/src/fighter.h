#ifndef FIGHTER_H__
#define FIGHTER_H__

#include <string>
#include <memory>
#include <vector>

class Skill;
typedef std::shared_ptr<Skill> Skill_ptr;

struct Damage;

class Actor;
typedef std::shared_ptr<Actor> Actor_ptr;

class Fighter
{
public:
	Fighter();
	Damage TakeDamage(Damage& dmg);
	Damage ApplyHealing(Damage& heal);
	void DamageModifiers(Damage& dmg, bool critting);
	void ApplyLethal();
	bool RespectsTargeting(Actor_ptr ap, int tm);
	void TurnStart(std::vector<Actor_ptr>& actors);
	void SpecialActionCommand(Damage& dmg);
	void PhysicalOffenseActionCommand(Damage& dmg);
	void PhysicalDefenseActionCommand(Damage& dmg);
	void MagicalOffenseActionCommand(Damage& dmg);
	void MagicalDefenseActionCommand(Damage& dmg);

public:
	int DefenseDamageModification(bool critting);

public:
	bool Dead;
	Actor_ptr Protector;
	int Team;
	bool Selected;
	bool ChoosingAction;
	std::vector<Skill_ptr> Skills;

protected:
	void SetDefault();
};


#endif