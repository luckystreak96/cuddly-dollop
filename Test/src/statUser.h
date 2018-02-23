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
	int* GetEndurancePointer();
	void SetMaxHealth(int maxHealth);
	int GetMaxHealth();
	int* GetMaxHealthPointer();

	// Stat points + nextlevelexp
	void LevelUp();
	int CalculateNextLevelExp();

public:
	int Health;
	int Speed;
	int Strength;
	int Defense;
	int Crit;
	int NextLevelExp;
	int SkillPoints;
	std::string Curve;

protected:
	int Level;
	int Exp;
	int MaxHealth;
	int Endurance;
	void SetDefault();
};


#endif