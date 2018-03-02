#ifndef STAT_USER_H__
#define STAT_USER_H__

#include <string>
#include <memory>
#include <vector>

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

class StatUser
{
public:
	StatUser();

public:
	void SetLevel(int level);
	int GetLevel();
	// Handles level up logic
	void GiveExp(int exp);
	// Doesn't handle level up logic
	void SetExp(int exp);
	int GetExp();
	void SetEndurance(int endurance);
	Stat GetEndurance();
	Stat* GetEndurancePointer();
	void SetMaxHealth(int maxHealth);
	Stat GetMaxHealth();
	Stat* GetMaxHealthPointer();
	// Makes sure current HP isnt greater than the max
	void CurrentHealthCheck();
	void ResetRealModified();
	void ResetModified();

	// Stat points + nextlevelexp
	void LevelUp();
	int CalculateNextLevelExp();

public:
	int Health;
	Stat Speed;
	Stat Strength;
	Stat Defense;
	Stat Crit;
	int NextLevelExp;
	int SkillPoints;
	std::string Curve;

protected:
	int Level;
	int Exp;
	Stat MaxHealth;
	Stat Endurance;
	void SetDefault();
};


#endif