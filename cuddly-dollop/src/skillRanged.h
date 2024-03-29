#ifndef SKILL_RANGED_H__
#define SKILL_RANGED_H__

#include "skill.h"
#include "vector3f.h"

class SkillRanged : public Skill
{

public:
	SkillRanged();
	//virtual void Start();
	//virtual BattleState Setup(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner);
	//virtual void Update();
	virtual bool IsReady();
	//virtual void Animate();
	//virtual void ApplyEffect();
	virtual bool TimingCondition(double progress, double duration);
	virtual void SetAnimations();
	virtual Damage CalculateDamage(StatUser& user);

protected:
	virtual void DefaultSetup();
	//Vector3f _basePos;
	//Vector3f _cameraTarget;
	//int _targetProgress;
};

#endif
