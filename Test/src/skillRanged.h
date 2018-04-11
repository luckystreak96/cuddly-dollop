#ifndef SKILL_RANGED_H__
#define SKILL_RANGED_H__

#include "skill.h"
#include "vector3f.h"

class SkillRanged : public Skill
{

public:
	SkillRanged();
	virtual BattleState Start(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner);
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
