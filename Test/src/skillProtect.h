#ifndef SKILL_PROTECT_H__
#define SKILL_PROTECT_H__

#include "skillMelee.h"
#include "vector3f.h"

class SkillProtect : public SkillMelee
{

public:
	using SkillMelee::SkillMelee;
	virtual void ApplyEffect();

protected:
	virtual void DefaultSetup();
};

#endif
