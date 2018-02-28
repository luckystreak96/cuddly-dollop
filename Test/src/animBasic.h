#ifndef ANIM_BASIC_H__
#define ANIM_BASIC_H__

#include "battleAnimation.h"
#include "vector3f.h"
#include "actor.h"

class AnimBasic : public BattleAnimation
{
public:
	AnimBasic(Anim_Enum anim, Actor_ptr target, double seconds);
	virtual void Update();

public:
	Actor_ptr _target;
	Anim_Enum _anim;
	int _start;
	int _prevState;
	int _length;
};

#endif // !BATTLE_ANIMATION_H__
