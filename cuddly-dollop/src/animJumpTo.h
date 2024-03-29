#ifndef ANIM_JUMP_TO_H__
#define ANIM_JUMP_TO_H__

#include "battleAnimation.h"
#include "vector3f.h"
#include "actor.h"

class AnimJumpTo : public BattleAnimation
{
public:
	// battle version
	AnimJumpTo(bool jumpToActivePosition, Actor_ptr owner);
	AnimJumpTo(Vector3f destination, Actor_ptr target);
	virtual void Update();
	void first_time();

public:
	Actor_ptr _target;
	Vector3f _destination;
	Vector3f _initialPos;
	Vector3f _speed;

	// Animation length
	int _length;
	int _start;
};

#endif // !BATTLE_ANIMATION_H__
