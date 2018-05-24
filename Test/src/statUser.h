#ifndef STAT_USER_H__
#define STAT_USER_H__

#include <string>
#include <memory>
#include <vector>

#include "observable.h"

class Stat
{
public:
	Stat();

	// Sets Base stat
	void operator=(int value);

public:
	// The base stat of the fighter determined by his stat curve only
	int Base;
	// The stat after passive skill and equipment application
	int Real;
	// The stat in battle - subject to temporary stat modifications
	int Modified;
};

class StatUser : public Observable
{
public:
	StatUser();

public:
	virtual void SetLevel(int level);
	int GetLevel();
	// Handles level up logic
	virtual void GiveExp(int exp);
	// Doesn't handle level up logic
	virtual void SetExp(int exp);
	int GetExp();

	// Endurance
	void SetEndurance(int endurance);
	Stat GetEndurance();
	Stat* GetEndurancePointer();

	// Maxhealth
	void SetMaxHealth(int maxHealth);
	Stat GetMaxHealth();
	Stat* GetMaxHealthPointer();

	// Makes sure current HP isnt greater than the max
	void CurrentHealthCheck();

	// Reset the modified stats to their base form
	void ResetRealModified();
	void ResetModified();

	// Stat points + nextlevelexp
	virtual void LevelUp();
	int CalculateLevelExp(int level);
	int CalculateNextLevelExp();

public:
	int Health;
	Stat Speed;
	Stat Strength;
	Stat Defense;
	Stat Crit;
	int NextLevelExp;
	int SkillPoints;
	int DeathExp;
	std::string Curve;

protected:
	int Level;
	int Exp;
	Stat MaxHealth;
	Stat Endurance;
	void SetDefault();
};

#endif