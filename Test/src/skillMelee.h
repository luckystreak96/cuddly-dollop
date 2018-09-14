#ifndef SKILL_MELEE_H__
#define SKILL_MELEE_H__

#include "skill.h"
#include "vector3f.h"

class SkillMelee: public Skill
{

public:
	using Skill::Skill;
	virtual void Start();
	virtual BattleState Setup();
	virtual void Update();
	virtual bool IsReady();
	virtual void Animate();
	virtual void ApplyEffect();
	virtual bool TimingCondition(double progress, double duration);

protected:
	void DefaultSetup();
	Vector3f _basePos;
};

#endif
