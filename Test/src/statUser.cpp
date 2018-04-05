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
	DeathExp = 8;
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

void StatUser::ResetRealModified()
{
	Speed.Real = Speed.Base;
	Strength.Real = Strength.Base;
	Defense.Real = Defense.Base;
	Crit.Real = Crit.Base;
	MaxHealth.Real = MaxHealth.Base;
	Endurance.Real = Endurance.Base;

	ResetModified();
}

void StatUser::ResetModified()
{
	Speed.Modified = Speed.Real;
	Strength.Modified = Strength.Real;
	Defense.Modified = Defense.Real;
	Crit.Modified = Crit.Real;
	MaxHealth.Modified = MaxHealth.Real;
	Endurance.Modified = Endurance.Real;
}

void StatUser::SetEndurance(int endurance)
{
	Endurance = endurance;
	SetMaxHealth(5 + endurance * 3 * (1.0f + (float)endurance / 100.0f));
}

void StatUser::SetMaxHealth(int max)
{
	// Cant use this -- set max health doesnt take into account passives
	//if (Health > max)
	//	Health = max;

	MaxHealth = max;
}

void StatUser::CurrentHealthCheck()
{
	if (Health > MaxHealth.Real)
		Health = MaxHealth.Real;
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
	Level++;
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
	while (Exp > NextLevelExp)
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
