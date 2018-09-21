#ifndef SKILL_HEAL_H__
#define SKILL_HEAL_H__

#include "skillMelee.h"
#include "vector3f.h"

class SkillHeal : public SkillMelee
{

public:
	using SkillMelee::SkillMelee;
	//virtual void ApplyEffect();
	//virtual Damage CalculateDamage();

protected:
	virtual void DefaultSetup();
};

#endif
