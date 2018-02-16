#ifndef ACTOR_H__
#define ACTOR_H__

#include <string>
#include <memory>
#include <vector>
#include "playerGraphicsComponent.h"

class Skill;
typedef std::shared_ptr<Skill> Skill_ptr;

struct Damage;

class Actor;
typedef std::shared_ptr<Actor> Actor_ptr;

class Actor : public PlayerGraphicsComponent
{
public:
	Actor();
	Damage TakeDamage(Damage& dmg);
	Damage ApplyHealing(Damage& heal);
	void DamageModifiers(Damage& dmg, bool critting);
	void ApplyLethal();
	void SetColor();
	bool RespectsTargeting(Actor_ptr ap, int tm);
	void Update();
	void TurnStart(std::vector<Actor_ptr>& actors);
	void SpecialActionCommand(Damage& dmg);
	void PhysicalOffenseActionCommand(Damage& dmg);
	void PhysicalDefenseActionCommand(Damage& dmg);
	void MagicalOffenseActionCommand(Damage& dmg);
	void MagicalDefenseActionCommand(Damage& dmg);

public:
	void SetLevel(int level);
	int GetLevel();
	// Handles level up logic
	void GiveExp(int exp);
	// Doesn't handle level up logic
	void SetExp(int exp);
	int GetExp();
	void SetEndurance(int endurance);
	int GetEndurance();
	void SetMaxHealth(int maxHealth);
	int GetMaxHealth();
	int DefenseDamageModification(bool critting);

	void LevelUp();
	int GetNextLevelExp();

public:
	std::string Name;
	int Health;
	int Speed;
	int Strength;
	int Defense;
	int Crit;
	bool Dead;
	int NextLevelExp;
	int SkillPoints;
	Actor_ptr Protector;
	int Team;
	bool Selected;
	bool ChoosingAction;
	std::string Sprite;
	std::vector<Skill_ptr> Skills;
	Vector3f BasePosition;
	static inline bool ActorSpeedSort(Actor_ptr a, Actor_ptr b) { return a->Speed > b->Speed; }

protected:
	int Level;
	int Exp;
	int MaxHealth;
	int Endurance;
	void SetDefault();
};

#endif