#include "statUser.h"

#include "actor.h"
#include "skill.h"
#include "skillSmack.h"

StatUser::StatUser()
{
	SetDefault();
}

void StatUser::SetDefault()
{
	Health = 10;
	MaxHealth = 10;
	Speed = 0;
	Strength = 0;
	Defense = 0;
	Endurance = 0;
	NextLevelExp = 0;
	Level = 1;
	Crit = 0;
	Curve = "Basic";
}

void StatUser::SetEndurance(int endurance)
{
	Endurance = endurance;
	SetMaxHealth(5 + endurance * 3 * (1.0f + (float)endurance / 100.0f));
}

void StatUser::SetMaxHealth(int max)
{
	if (Health > max)
		Health = max;

	MaxHealth = max;
}

Stat StatUser::GetEndurance()
{
	return Endurance;
}

Stat* StatUser::GetEndurancePointer()
{
	return &Endurance;
}

void StatUser::SetLevel(int level)
{
	Level = level;
	NextLevelExp = CalculateNextLevelExp();
}

void StatUser::LevelUp()
{
	SkillPoints += 4;
	NextLevelExp = CalculateNextLevelExp();
}

int StatUser::CalculateNextLevelExp()
{
	return 10 * Level + pow(Level, 2);
}

int StatUser::GetLevel()
{
	return Level;
}

void StatUser::GiveExp(int exp)
{
	Exp += exp;
	while (exp > NextLevelExp)
	{
		// Level up
		LevelUp();
	}
}

void StatUser::SetExp(int exp)
{
	Exp = exp;
}

int StatUser::GetExp()
{
	return Exp;
}

Stat StatUser::GetMaxHealth()
{
	return MaxHealth;
}

Stat* StatUser::GetMaxHealthPointer()
{
	return &MaxHealth;
}

Stat::Stat()
{
	Base = 0;
	Real = 0;
	Modified = 0;
}

void Stat::operator=(int value)
{
	Base = value;
	Real = value;
	Modified = value;
}
