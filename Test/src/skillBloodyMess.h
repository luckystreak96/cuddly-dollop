#ifndef SKILL_BLOODY_MESS_H__
#define SKILL_BLOODY_MESS_H__

#include "skillRanged.h"
#include "vector3f.h"

class SkillBloodyMess : public SkillRanged
{

public:
	SkillBloodyMess();
	virtual void ApplyEffect();
	Damage CalculateDamage();

protected:
	void DefaultSetup();
};

#endif