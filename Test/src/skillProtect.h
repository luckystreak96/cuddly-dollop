#ifndef SKILL_PROTECT_H__
#define SKILL_PROTECT_H__

#include "skillMelee.h"
#include "vector3f.h"

class SkillProtect : public SkillMelee
{

public:
	SkillProtect();
	virtual void ApplyEffect();

protected:
	void DefaultSetup();
};

#endif
