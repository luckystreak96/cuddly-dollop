#ifndef SKILL_HEAL_H__
#define SKILL_HEAL_H__

#include "skillMelee.h"
#include "vector3f.h"

class SkillHeal : public SkillMelee
{

public:
	SkillHeal();
	//virtual void ApplyEffect();
	virtual Damage CalculateDamage(StatUser& user);

protected:
	virtual void DefaultSetup();
	virtual void ModifyAnimations();
};

#endif
