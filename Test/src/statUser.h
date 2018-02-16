#ifndef STAT_USER_H__
#define STAT_USER_H__

#include <string>
#include <memory>
#include <vector>

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
	int GetEndurance();
	void SetMaxHealth(int maxHealth);
	int GetMaxHealth();

	// Stat points + nextlevelexp
	void LevelUp();
	int CalculateNextLevelExp();

public:
	int Health;
	int Speed;
	int Strength;
	int Defense;
	int Crit;
	bool Dead;
	int NextLevelExp;
	int SkillPoints;

protected:
	int Level;
	int Exp;
	int MaxHealth;
	int Endurance;
	void SetDefault();
};


#endif