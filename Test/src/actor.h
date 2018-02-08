#ifndef ACTOR_H__
#define ACTOR_H__

#include <string>
#include <memory>
#include <vector>
#include "playerGraphicsComponent.h"

class Skill;
typedef std::shared_ptr<Skill> Skill_ptr;

class Actor;
typedef std::shared_ptr<Actor> Actor_ptr;

class Actor : public PlayerGraphicsComponent
{
public:
	Actor();
	int TakeDamage(int& dmg);
	int ApplyHealing(int& heal);
	void ApplyLethal();
	void SetColor();
	bool RespectsTargeting(Actor_ptr ap, int tm);
	void Update();
	void TurnStart(std::vector<Actor_ptr>& actors);
	void SpecialActionCommand(int& dmg);
	void PhysicalOffenseActionCommand(int& dmg);
	void PhysicalDefenseActionCommand(int& dmg);
	void MagicalOffenseActionCommand(int& dmg);
	void MagicalDefenseActionCommand(int& dmg);
	void SetEndurance(int endurance);
	int GetEndurance();
	void SetMaxHealth(int maxHealth);
	int GetMaxHealth();
	void DamageModifiers(int& dmg, bool critting);
	int DefenseDamageModification(bool critting);

public:
	std::string Name;
	int Health;
	int Speed;
	int Strength;
	int Defense;
	int Crit;
	bool Dead;
	Actor_ptr Protector;
	int Team;
	bool Selected;
	bool ChoosingAction;
	std::vector<Skill_ptr> Skills;
	Vector3f BasePosition;
	static inline bool ActorSpeedSort(Actor_ptr a, Actor_ptr b) { return a->Speed > b->Speed; }

protected:
	int MaxHealth;
	int Endurance;
	void SetDefault();
};

#endif