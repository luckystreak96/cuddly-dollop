#ifndef SKILL_SMACK_H__
#define SKILL_SMACK_H__

#include "skillMelee.h"
#include "vector3f.h"

class SkillSmack : public SkillMelee
{

public:
	SkillSmack();
	//virtual void ApplyEffect();
	Damage CalculateDamage(StatUser& user);

protected:
	virtual void DefaultSetup();
};

#endif