#ifndef ANIM_MOVE_TO_H__
#define ANIM_MOVE_TO_H__

#include "battleAnimation.h"
#include "vector3f.h"
#include "actor.h"

class AnimMoveTo : public BattleAnimation
{
public:
	AnimMoveTo(Vector3f destination, Actor_ptr target);
	AnimMoveTo(Vector3f destination, Actor_ptr target, double duration);
	virtual void Update();

public:
	Actor_ptr _target;
	Vector3f _destination;
	Vector3f _initialPos;
	Vector3f _speed;
	Anim_Enum _prevDir;
};

#endif // !BATTLE_ANIMATION_H__
