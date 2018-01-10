#ifndef SKILL_SMACK_H__
#define SKILL_SMACK_H__

#include "skill.h"
#include "vector3f.h"

class SkillSmack : public Skill
{

public:
	SkillSmack();
	virtual BattleState Start(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner);
	virtual void Update();
	virtual bool IsReady();

protected:
	void DefaultSetup();
	Vector3f _basePos;
};

#endif