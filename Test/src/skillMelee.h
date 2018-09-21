#ifndef SKILL_MELEE_H__
#define SKILL_MELEE_H__

#include "skill.h"
#include "vector3f.h"

class SkillMelee: public Skill
{

public:
	SkillMelee();
	//virtual void Start();
	//virtual void Setup();
	//virtual SkillProgress Update();
	virtual bool IsReady();
	//virtual void Animate();
	//virtual void ApplyEffect();
	virtual bool TimingCondition(double progress, double duration);
	virtual void SetAnimations();

protected:
	virtual void DefaultSetup();
	Vector3f _basePos;
};

#endif
